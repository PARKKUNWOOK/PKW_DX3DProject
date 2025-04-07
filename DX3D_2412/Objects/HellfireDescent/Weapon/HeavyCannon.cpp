#include "Framework.h"

HeavyCannon::HeavyCannon(Transform* transform)
{
    weaponType = WeaponType::HeavyCannon;

    SetTag(transform->GetTag() + "_Collider");
    transform->SetParent(this);
    transform->SetTag("HeavyCannon_0");
    transform->Load();

    damage = 1;
    fireInterval = 0.1f;
    
}

HeavyCannon::~HeavyCannon()
{
}

void HeavyCannon::HandleInput()
{
    fireTimer += DELTA;

    if (!KEY->Press(VK_LBUTTON)) return;
    if (fireTimer < fireInterval) return;

    WeaponManager::Get()->Fire(firePosition, fireDirection, weaponType);
    fireTimer = 0.0f;
}