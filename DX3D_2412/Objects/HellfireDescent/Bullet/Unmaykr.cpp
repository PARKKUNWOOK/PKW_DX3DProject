#include "Framework.h"

Unmaykr::Unmaykr(Transform* transform)
	: Bullet(transform)
{
	SetTag("RedBullet");
	transform->SetTag("RedBullet_0");
	transform->SetParent(this);
	transform->Load();

	damage = 2;
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
	Bullet::Render();
}

void Unmaykr::Fire(Vector3 pos, Vector3 dir, bool isPlayer)
{
    Vector3 right = Vector3::Cross(Vector3::Up(), dir).GetNormalized();

    Vector3 dirCenter = dir;
    Vector3 dirLeft = (dir - right * 0.2f).GetNormalized();
    Vector3 dirRight = (dir + right * 0.2f).GetNormalized();

    int firedCount = 0;

    for (Bullet* bullet : BulletManager::Get()->GetBullets())
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

                unmaykr->Bullet::Fire(pos, fireDir, isPlayer);
                firedCount++;
            }
        }

        if (firedCount >= 3) break;
    }
}
