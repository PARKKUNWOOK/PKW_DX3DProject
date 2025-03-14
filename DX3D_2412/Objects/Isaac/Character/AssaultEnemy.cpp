#include "Framework.h"

AssaultEnemy::AssaultEnemy()
{
	model = new ModelAnimator("Monster1");
	model->SetShader(L"Model/Model.hlsl");
	model->ReadClip("Run");
	model->ReadClip("Attack");
	model->ReadClip("Death");
	model->CreateTexture();

	model->GetClip(2)->SetEvent(bind(&AssaultEnemy::Death, this), 0.8f);

	model->SetParent(this);
	model->SetLocalPosition(0.0f, -1.0f, 0.0f);
	model->SetLocalScale(0.01f, 0.01f, 0.01f);
	model->SetLocalRotation(0, XM_PI, 0);

	model->PlayClip(0);
}

AssaultEnemy::~AssaultEnemy()
{
	delete model;
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
			model->PlayClip(0);
		}
	}
	else
	{
		FollowTarget();
	}

	UpdateWorld();

	for (Bullet* bullet : Player::Get()->GetBullets()->GetAllActive())
	{
		if (bullet->AssaultEnemyCollisionCheck(this))
		{
			bullet->SetActive(false);
			TakeDamage(1);
			return;
		}
	}

	AssaultAttack(Player::Get());

	model->Update();
}

void AssaultEnemy::Render()
{
	if (!IsActive()) return;

	Collider::Render();
	model->Render();
}

void AssaultEnemy::PostRender()
{
}

void AssaultEnemy::Edit()
{
	Collider::Edit();
	model->Edit();
}

void AssaultEnemy::SetTarget(Transform* target)
{
	this->target = target;
}

void AssaultEnemy::Reset()
{
	SetActive(false);
}

void AssaultEnemy::TakeDamage(int damage)
{
	if (curHp <= 0) return;

	curHp -= damage;
	if (curHp < 0) curHp = 0;

	if (curHp <= 0)
	{
		isDying = true;
		model->PlayClip(2);

		//model->GetClip(2)->SetEvent([this]() 
		//{
		//	this->SetActive(false);
		//	this->isDying = false;
		//}, 1.0f);
	}
}

void AssaultEnemy::AssaultAttack(Player* player)
{
	if (!IsActive() || player->IsGameOver()) return;

	if (player->IsCollision(this) && !isAssaultAttacking)
	{
		Vector3 knockbackDir = player->GetLocalPosition() - GetLocalPosition();
		player->TakeDamage(1, knockbackDir);

		model->PlayClip(1);
		isAssaultAttacking = true;
		assaultAttackFrameCount = 1000;
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

void AssaultEnemy::Death()
{
	SetActive(false);
}
