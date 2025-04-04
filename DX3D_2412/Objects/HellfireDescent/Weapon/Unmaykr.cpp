#include "Framework.h"

Unmaykr::Unmaykr(Transform* transform)
{
    weaponType = WeaponType::Unmaykr;

    SetTag(transform->GetTag() + "_Collider");
	transform->SetParent(this);
    //transform->SetLocalPosition(Vector3(18.0f, 0.0f, 0.5f));
    transform->SetTag("Unmaykr_0");
	transform->Load();

	damage = 2;
    fireInterval = 0.1f;
}

Unmaykr::~Unmaykr()
{
}

void Unmaykr::Update()
{
    if (!IsActive()) return;

    lifeTime += DELTA;

    if (lifeTime > LIFE_TIME)
        SetActive(false);

    Translate(velocity * speed * DELTA);

    UpdateWorld();
}

void Unmaykr::Render()
{
    Weapon::Render();
}

void Unmaykr::Fire(Vector3 pos, Vector3 dir)
{
    Vector3 right = Vector3::Cross(Vector3::Up(), dir).GetNormalized();

    Vector3 dirCenter = dir;
    Vector3 dirLeft = (dir - right * 0.2f).GetNormalized();
    Vector3 dirRight = (dir + right * 0.2f).GetNormalized();

    int firedCount = 0;

    for (Weapon* bullet : WeaponManager::Get()->GetWeapons())
    {
        if (!bullet->IsActive())
        {
            Unmaykr* unmaykr = dynamic_cast<Unmaykr*>(bullet);
            if (unmaykr)
            {
                Vector3 fireDir;
                if (firedCount == 0) fireDir = dirLeft;
                else if (firedCount == 1) fireDir = dirCenter;
                else if (firedCount == 2) fireDir = dirRight;
                else break;

                unmaykr->Weapon::Fire(pos, fireDir);
                firedCount++;
            }
        }

        if (firedCount >= 3) break;
    }
}

void Unmaykr::HandleInput()
{
    fireTimer += DELTA;

    if (!KEY->Press(VK_LBUTTON)) return;
    if (fireTimer < fireInterval) return;

    WeaponManager::Get()->Fire(firePosition, fireDirection, weaponType);
    fireTimer = 0.0f;
}
