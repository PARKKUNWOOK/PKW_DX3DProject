#include "Framework.h"

int Enemy::enemyCount = 0;

Enemy::Enemy()
{
	cube = new Cube(Vector3(1.5f, 1.5f, 1.5f));
	cube->SetParent(this);
	cube->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Floor.png");

	id = ++enemyCount;

	hpBar = new Quad(Vector2(3.0f, 0.3f));
	hpBar->GetMaterial()->SetDiffuseMap(L"Resources/Textures/HpBar/HpBar.png");
	hpBar->SetPivot(Vector3(-1.0f, 3.0f, 0.0f));
	hpBar->SetLocalRotation(Vector3(0.0f, 0.0f, 0.0f));
}

Enemy::~Enemy()
{
	delete cube;
	delete hpBar;
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

	FollowTarget();
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

	cube->UpdateWorld();

	Vector3 basePosition = localPosition + Vector3(0, 7.0f, 0);
	
	float spacing = 0.4f;
	float startX = basePosition.x - (spacing * 4.5f);
	
	for (int i = 0; i < 10; i++)
	{
		Vector3 hpPos = { startX + (i * spacing), basePosition.y, basePosition.z };
		hpBar->SetLocalPosition(hpPos);
		//hpBar->LookAt(CAM->GetLocalPosition());
		hpBar->UpdateWorld();
	}

	direction = target->GetLocalPosition() - localPosition;
	float distance = direction.Magnitude();

	float angle = atan2(direction.x, direction.z);
	hpBar->SetLocalRotation(Vector3::Up() * angle);
}

void Enemy::Render()
{
	Collider::Render();
	cube->Render();

	for (int i = 0; i < 10; i++)
	{
		hpBar->Render();
	}
}

void Enemy::PostRender()
{
	if (!IsActive()) return;

	Vector3 namePosition = localPosition + Vector3(0, 2.0f, 0);
	Vector2 screenPos = CAM->WorldToScreenPoint(namePosition);

	string enemyName = "Enemy" + to_string(id);
	Font::Get()->RenderText(enemyName, screenPos);
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

	float hpRatio = (float)curHp / maxHp;
	hpBar->SetLocalScale(Vector3(hpRatio, 1.0f, 1.0f));

	if (curHp <= 0)
	{
		isDying = true;
	}
}

void Enemy::FollowTarget()
{
	if (target == nullptr || isDying) return;

	direction = target->GetLocalPosition() - localPosition;
	float distance = direction.Magnitude();

	float angle = atan2(direction.x, direction.z);
	SetLocalRotation(Vector3::Up() * angle);

	if (distance > 0.1f)
	{
		direction.Normalize();
		localPosition += direction * speed * DELTA;
	}
}
