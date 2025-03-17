#include "Framework.h"

AssaultEnemy::AssaultEnemy()
{
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
	maxHp = curHp = 2;
}

AssaultEnemy::~AssaultEnemy()
{
}

void AssaultEnemy::Update()
{
	if (!IsActive()) return;

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
			else
			{
				TakeDamage(1);
			}
		}
	}
	else
	{
		FollowTarget();
	}

	Enemy::Update();

	for (Bullet* bullet : Player::Get()->GetBullets()->GetAllActive())
	{
		if (bullet->AssaultEnemyCollisionCheck(this))
		{
			bullet->SetActive(false);
			TakeDamage(1);
			return;
		}
	}

	Attack(Player::Get());
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
			model->PlayClip(1);
			isAssaultAttacking = true;
			assaultAttackFrameCount = 1000;
		}
		else
		{
			TakeDamage(1);
		}
		
	}
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
