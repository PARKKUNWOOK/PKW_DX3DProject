#include "Framework.h"

Bullet::Bullet() : SphereCollider(0.2f)
{
	sphere = new Sphere(0.2f);
	sphere->SetParent(this);
	sphere->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Block/Block1.png");
}

Bullet::~Bullet()
{
	delete sphere;
}

void Bullet::Update()
{
	if (!IsActive()) return;

	lifeTime += DELTA;

	if (lifeTime > LIFE_TIME)
		SetActive(false);

	Translate(velocity * speed * DELTA);

	if (!isPlayerBullet)
	{
		if (PlayerCollisionCheck(Player::Get()))
		{
			SetActive(false);
			return;
		}
	}

	UpdateWorld();
	sphere->UpdateWorld();
}

void Bullet::Render()
{
	Collider::Render();
	sphere->Render();
}

void Bullet::Fire(Vector3 pos, Vector3 dir, bool isPlayer)
{
	localPosition = pos;
	velocity = dir;
	lifeTime = 0.0f;
	isPlayerBullet = isPlayer;

	SetActive(true);
}

bool Bullet::EnemyCollisionCheck(Enemy* enemy)
{
	if (!IsActive() || !enemy->IsActive()) return false;

	if (Intersects(enemy))
	{
		enemy->TakeDamage(damage);
		return true;
	}

	return false;
}

bool Bullet::PlayerCollisionCheck(Player* player)
{
	if (!player || !IsActive()) return false;

	if (player->IsCollision(this))
	{
		Vector3 knockbackDir = direction;
		if (knockbackDir.Magnitude() < 0.01f)
		{
			knockbackDir = player->GetLocalPosition() - GetLocalPosition();
		}

		knockbackDir.Normalize();
		player->TakeDamage(1, knockbackDir);

		SetActive(false);
		return true;
	}

	return false;
}
