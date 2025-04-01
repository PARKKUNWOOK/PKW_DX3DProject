#include "Framework.h"

Weapon::Weapon() : SphereCollider(0.9f)
{
}

Weapon::~Weapon()
{
}

void Weapon::Update()
{
	if (!IsActive()) return;

	lifeTime += DELTA;

	if (lifeTime > LIFE_TIME)
		SetActive(false);

	Translate(velocity * speed * DELTA);

	UpdateWorld();
}

void Weapon::Render()
{
	Collider::Render();
}

void Weapon::Edit()
{
	Transform::Edit();
}

void Weapon::Fire(Vector3 pos, Vector3 dir)
{
	localPosition = pos;
	velocity = dir;
	lifeTime = 0.0f;

	localRotation.y = atan2(dir.x, dir.z);
	localRotation.y += 4.75f;
	localRotation.z = asin(dir.y);

	SetActive(true);
}

bool Weapon::EnemyCollisionCheck(Enemy* enemy)
{
	if (!IsActive() || !enemy->IsActive()) return false;
	if (enemy->GetCurrentHealth() <= 0) return false;

	if (Intersects(enemy))
	{
		return true;
	}

	return false;
}
