#include "Framework.h"

PlayerBullet::PlayerBullet(Transform* transform)
	: Bullet()
{
	this->transform = transform;
	transform->SetTag("Bullet_0");
	transform->SetParent(this);
	transform->Load();
}

void PlayerBullet::Update()
{
    if (!IsActive()) return;

    lifeTime += DELTA;
    if (lifeTime > LIFE_TIME)
    {
        SetActive(false);
        return;
    }

    Translate(velocity * speed * DELTA);
    UpdateWorld();
    transform->UpdateWorld();
}
