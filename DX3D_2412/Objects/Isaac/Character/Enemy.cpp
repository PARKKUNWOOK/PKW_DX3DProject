#include "Framework.h"

int Enemy::enemyCount = 0;

Enemy::Enemy()
{
	model = new ModelAnimator("Monster1");
	model->SetShader(L"Model/Model.hlsl");
	model->ReadClip("Run");
	model->ReadClip("Attack");
	model->CreateTexture();

	model->SetParent(this);
	model->SetLocalPosition(0.0f, -1.0f, 0.0f);
	model->SetLocalScale(0.01f, 0.01f, 0.01f);
	model->SetLocalRotation(0, XM_PI, 0);

	model->PlayClip(0);

	id = ++enemyCount;
}

Enemy::~Enemy()
{
	delete model;
}

void Enemy::Update()
{
	if (!IsActive()) return;

	if (isDying)
	{
		SetActive(false);
		isDying = false;

		return;
	}

	if (isAttacking)
	{
		attackFrameCount--;

		if (attackFrameCount <= 0)
		{
			isAttacking = false;
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
		if (bullet->CollisionCheck(this))
		{
			bullet->SetActive(false);
			TakeDamage(1);
			return;
		}
	}

	Attack(Player::Get());

	model->Update();
}

void Enemy::Render()
{
	Collider::Render();
	model->Render();
}

void Enemy::PostRender()
{
	if (!IsActive()) return;

	Vector3 namePosition = localPosition + Vector3(0, 2.0f, 0);
	Vector2 screenPos = CAM->WorldToScreenPoint(namePosition);

	string enemyName = "Enemy" + to_string(id);
	Font::Get()->RenderText(enemyName, screenPos);
}

void Enemy::Edit()
{
	Collider::Edit();
	model->Edit();
}

void Enemy::SetTarget(Transform* target)
{
	this->target = target;
}

void Enemy::Reset()
{
	SetActive(false);
}

void Enemy::TakeDamage(int damage)
{
	if (curHp <= 0) return;

	curHp -= damage;
	if (curHp < 0) curHp = 0;

	if (curHp <= 0)
	{
		isDying = true;
	}
}

void Enemy::Attack(Player* player)
{
	if (!IsActive() || player->IsGameOver()) return;

	if (player->IsCollision(this) && !isAttacking)
	{
		Vector3 knockbackDir = player->GetLocalPosition() - GetLocalPosition();
		player->TakeDamage(1, knockbackDir);

		model->PlayClip(1);
		isAttacking = true;
		attackFrameCount = 680;
	}
}

void Enemy::FollowTarget()
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
