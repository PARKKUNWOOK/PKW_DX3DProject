#include "Framework.h"

RocketLauncher::RocketLauncher(Transform* transform)
{
	weaponType = WeaponType::RocketLauncher;

	SetTag(transform->GetTag() + "_Collider");
	transform->SetParent(this);
	//transform->SetLocalPosition(Vector3(2.0f, 6.0f, 0.5f));
	transform->SetTag("RocketLauncher_0");
	transform->Load();

	damage = 10;
	fireInterval = 2.0f;
}

RocketLauncher::~RocketLauncher()
{
}

void RocketLauncher::Update()
{
	if (!IsActive()) return;

	if (exploded)
	{
		explosionTime -= DELTA;
		if (explosionTime <= 0.0f)
		{
			SetActive(false);
		}
	}
	else
	{
		lifeTime += DELTA;
		if (lifeTime > LIFE_TIME)
		{
			SetActive(false);
			return;
		}

		Translate(velocity * speed * DELTA);
		UpdateWorld();
	}
}

void RocketLauncher::Render()
{
	Weapon::Render();
}

void RocketLauncher::HandleInput()
{
	fireTimer += DELTA;

	if (!KEY->Down(VK_LBUTTON)) return;
	if (fireTimer < fireInterval) return;

	WeaponManager::Get()->Fire(firePosition, fireDirection, weaponType);
	fireTimer = 0.0f;
}

void RocketLauncher::Fire(Vector3 pos, Vector3 dir)
{
	localPosition = pos;
	velocity = dir;
	lifeTime = 0.0f;
	exploded = false;
	radius = 0.9f;

	direction = dir;
	direction.Normalize();

	float yaw = atan2(dir.x, dir.z);
	float pitch = -asin(dir.y);

	localRotation.x = pitch + XM_PIDIV2;
	localRotation.y = yaw;

	SetActive(true);
	if (transform)
		transform->SetActive(true);
}

bool RocketLauncher::EnemyCollisionCheck(Enemy* enemy)
{
	if (!IsActive() || !enemy->IsActive()) return false;
	if (enemy->GetCurrentHealth() <= 0) return false;

	if (Intersects(enemy))
	{
		hitEnemy = enemy;
		Explode();
		return true;
	}

	return false;
}

void RocketLauncher::Explode()
{
	exploded = true;

	if (transform)
		transform->SetActive(false);

	radius = 999999999.0f;
	explosionTime = 0.1f;

	for (Enemy* enemy : EnemyManager::Get()->GetAllEnemies())
	{
		if (!enemy->IsActive()) continue;
		if (enemy == hitEnemy) continue;

		if (IsSphereCollision(enemy))
		{
			enemy->TakeDamage(5);
		}
	}

	hitEnemy = nullptr;
}
