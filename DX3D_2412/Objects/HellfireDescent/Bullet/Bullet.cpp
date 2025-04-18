#include "Framework.h"

Bullet::Bullet(Transform* transform) : SphereCollider(0.9f)
{
	SetTag(transform->GetTag() + "_Collider");
	transform->SetParent(this);
	transform->SetTag("FireBall_0");
	transform->Load();
}

Bullet::~Bullet()
{
	
}

void Bullet::Update()
{
	if (!IsActive()) return;

	lifeTime += DELTA;

	if (lifeTime > LIFE_TIME)
		SetActive(false);

	Translate(velocity * speed * DELTA);

	if (PlayerCollisionCheck(Player::Get()))
	{
		SetActive(false);
		return;
	}

	UpdateWorld();
}

void Bullet::Render()
{
	Collider::Render();
}

void Bullet::Edit()
{
	Transform::Edit();
}

void Bullet::Fire(Vector3 pos, Vector3 dir)
{
	localPosition = pos;
	velocity = dir;
	lifeTime = 0.0f;

	localRotation.y = atan2(dir.x, dir.z);
	localRotation.y += 4.75f;
	localRotation.z = asin(dir.y);

	SetActive(true);
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
