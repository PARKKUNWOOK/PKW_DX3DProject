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

    isActive = false;

    bloodEffect = new ParticleSystem("Resources/FX/Blood.fx");
    bloodEffect->SetActive(false);
}

Enemy::~Enemy()
{
    if (model) delete model;
    if (bloodEffect) delete bloodEffect;
}

void Enemy::Update()
{
    if (!IsActive()) return;

    AttackAction();
    Collider::Update();
    if (model) model->Update();

    UpdateWorld();

    if (bloodEffect) bloodEffect->Update();
}

void Enemy::Render()
{
    if (!IsActive()) return;

    Collider::Render();
    if (model) model->Render();

    if (bloodEffect) bloodEffect->Render();
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
        if (bloodEffect)
            bloodEffect->Play(GetGlobalPosition());
    }
}

void Enemy::Death()
{
    SetActive(false);
}
