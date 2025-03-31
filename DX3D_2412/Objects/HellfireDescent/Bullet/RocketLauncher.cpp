#include "Framework.h"

RocketLauncher::RocketLauncher(Transform* transform)
	: Bullet(transform)
{
	SetTag("Rocket");
	transform->SetTag("Rocket_0");
	transform->SetParent(this);
	transform->Load();

	damage = 10;
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
		return;
	}

	lifeTime += DELTA;
	if (lifeTime > LIFE_TIME)
	{
		SetActive(false);
		return;
	}

	Translate(velocity * speed * DELTA);
	UpdateWorld();
}

void RocketLauncher::Render()
{
	Bullet::Render();
}

void RocketLauncher::Fire(Vector3 pos, Vector3 dir, bool isPlayer)
{
	localPosition = pos;
	velocity = dir;
	lifeTime = 0.0f;
	isPlayerBullet = isPlayer;
	exploded = false;
	radius = 0.9f;

	direction = dir;
	direction.Normalize();

	float yaw = atan2(dir.x, dir.z);
	float pitch = -asin(dir.y);

	localRotation.x = pitch + XM_PIDIV2;
	localRotation.y = yaw;

	SetActive(true);
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

			if (enemy->GetCurrentHealth() == 0)
			{
				if (dynamic_cast<AssaultEnemy*>(enemy))
				{
					Audio::Get()->Add("AssaultDeath", "Resources/Sounds/HellfireDescentSound/AssaultEnemyDeath.ogg");
					Audio::Get()->Play("AssaultDeath", 3.0f);
				}
				else if (dynamic_cast<ThrowerEnemy*>(enemy))
				{
					Audio::Get()->Add("ThrowerDeath", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyDeath.ogg");
					Audio::Get()->Play("ThrowerDeath", 3.0f);
				}
			}
			else
			{
				if (dynamic_cast<AssaultEnemy*>(enemy))
				{
					Audio::Get()->Add("AssaultDamage", "Resources/Sounds/HellfireDescentSound/AssaultEnemyDamage.ogg");
					Audio::Get()->Play("AssaultDamage", 3.0f);
				}
				else if (dynamic_cast<ThrowerEnemy*>(enemy))
				{
					Audio::Get()->Add("ThrowerDamage", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyDamage.ogg");
					Audio::Get()->Play("ThrowerDamage", 3.0f);
				}
			}
		}
	}

	hitEnemy = nullptr;
}
