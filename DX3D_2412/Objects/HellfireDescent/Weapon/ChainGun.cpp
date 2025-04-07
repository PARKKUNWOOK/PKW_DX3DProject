#include "Framework.h"

ChainGun::ChainGun(Transform* transform)
{
    weaponType = WeaponType::ChainGun;

    SetTag(transform->GetTag() + "_Collider");
    transform->SetParent(this);
    transform->SetTag("ChainGun_0");
    transform->Load();

    damage = 1;

    fireInterval = minigunStartInterval;
}

ChainGun::~ChainGun()
{
}

void ChainGun::HandleInput()
{
    fireTimer += DELTA;

    if (KEY->Press(VK_LBUTTON))
    {
        minigunElapsed += DELTA;

        float t = minigunElapsed * minigunDecreaseRate;
        fireInterval = minigunStartInterval - t;

        if (fireInterval < minigunMinInterval)
            fireInterval = minigunMinInterval;

        if (fireTimer >= fireInterval)
        {
            WeaponManager::Get()->Fire(firePosition, fireDirection, weaponType);
            fireTimer = 0.0f;
        }
    }
    else
    {
        fireInterval = minigunStartInterval;
        minigunElapsed = 0.0f;
    }
}