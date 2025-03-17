#include "Framework.h"

Enemy::Enemy()
{
    target = nullptr;
    model = nullptr;

    speed = 0.0f;
    maxHp = 10;
    curHp = 10;
    isDying = false;

    Collider::SetLocalPosition(0.0f, 1.5f, 0.0f);
}

Enemy::~Enemy()
{
    if (model) delete model;
}

void Enemy::Update()
{
    if (!IsActive()) return;

    Collider::Update();
    if (model) model->Update();

    UpdateWorld();
}

void Enemy::Render()
{
    if (!IsActive()) return;

    Collider::Render();
    if (model) model->Render();
}

void Enemy::PostRender()
{
}

void Enemy::Edit()
{
    Collider::Edit();
    if (model) model->Edit();
}

void Enemy::SetTarget(Transform* target)
{
    this->target = target;
}

void Enemy::TakeDamage(int damage)
{
    if (curHp <= 0 || isDying) return;

    curHp -= damage;
    if (curHp < 0) curHp = 0;

    if (curHp == 0)
    {
        isDying = true;
        if (model) model->PlayClip(2);
    }
}

void Enemy::Death()
{
    SetActive(false);
}
