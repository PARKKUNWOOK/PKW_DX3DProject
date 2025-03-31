#include "Framework.h"

AssaultEnemy::AssaultEnemy()
{
	tag = "Assault";

	model = new ModelAnimator("Monster1");
	model->SetShader(L"Model/Model.hlsl");
	model->ReadClip("Run");
	model->ReadClip("Attack");
	model->ReadClip("Death");
	model->CreateTexture();

	model->GetClip(2)->SetEvent(bind(&AssaultEnemy::Death, this), 0.8);

	model->SetParent(this);
	model->SetLocalPosition(0.0f, -1.0f, 0.0f);
	model->SetLocalScale(0.01f, 0.01f, 0.01f);
	model->SetLocalRotation(0, XM_PI, 0);

	model->PlayClip(0);

	speed = 3.0f;
	//maxHp = curHp = 5;
	maxHp = curHp = 40;
}

AssaultEnemy::~AssaultEnemy()
{
}

void AssaultEnemy::Render()
{
	Enemy::Render();
}

void AssaultEnemy::Attack(Player* player)
{
	if (!IsActive() || player->IsGameOver() || isDying) return;

	if (player->IsCollision(this) && !isAssaultAttacking)
	{
		Vector3 knockbackDir = player->GetLocalPosition() - GetLocalPosition();
		player->TakeDamage(1, knockbackDir);

		if (!isDying)
		{
			Audio::Get()->Add("AssaultAttackVoice", "Resources/Sounds/HellfireDescentSound/AssaultEnemyAttackVoice.ogg");
			Audio::Get()->Play("AssaultAttackVoice", 3.0f);
			Audio::Get()->Add("AssaultAttack", "Resources/Sounds/HellfireDescentSound/AssaultEnemyAttack.ogg");
			Audio::Get()->Play("AssaultAttack", 5.0f);
			model->PlayClip(1);
			isAssaultAttacking = true;
			assaultAttackFrameCount = 1500;
		}
	}
}

void AssaultEnemy::AttackAction()
{
	if (isAssaultAttacking)
	{
		assaultAttackFrameCount--;

		if (assaultAttackFrameCount <= 0)
		{
			isAssaultAttacking = false;

			if (!isDying)
			{
				model->PlayClip(0);
			}
		}
	}
	else
	{
		FollowTarget();
	}

	for (Bullet* bullet : BulletManager::Get()->GetBullets())
	{
		if (!bullet->IsActive()) continue;

		if (bullet->IsPlayerBullet() && bullet->EnemyCollisionCheck(this))
		{
			TakeDamage(bullet->GetDamage());
			bullet->SetActive(false);
			if (curHp == 0)
			{
				Audio::Get()->Add("AssaultDeath", "Resources/Sounds/HellfireDescentSound/AssaultEnemyDeath.ogg");
				Audio::Get()->Play("AssaultDeath", 3.0f);
			}
			else
			{
				Audio::Get()->Add("AssaultDamage", "Resources/Sounds/HellfireDescentSound/AssaultEnemyDamage.ogg");
				Audio::Get()->Play("AssaultDamage", 3.0f);
			}
			return;
		}
	}

	Attack(Player::Get());
}

void AssaultEnemy::FollowTarget()
{
	if (target == nullptr || isDying) return;

	direction = target->GetLocalPosition() - localPosition;
	direction.y = 0.0f;

	float distance = direction.Magnitude();

	if (distance > 0.1f)
	{
		direction.Normalize();

		Vector3 newPosition = localPosition + direction * speed * DELTA;
		newPosition.y = localPosition.y;
		SetLocalPosition(newPosition);
	}

	if (distance > 0.01f)
	{
		float targetAngle = atan2(direction.x, direction.z);
		Vector3 currentRotation = GetLocalRotation();

		float newAngle = GameMath::Lerp(currentRotation.y, targetAngle, 0.1f);
		SetLocalRotation(Vector3(0.0f, newAngle, 0.0f));
	}
}
