#include "Framework.h"

CombatShotgun::CombatShotgun()
{
    weaponType = WeaponType::CombatShotgun;
    damage = 1;
}

CombatShotgun::~CombatShotgun()
{
}

void CombatShotgun::Fire(Vector3 pos, Vector3 dir)
{
    Vector3 forward = dir.GetNormalized();
    Vector3 right = Vector3::Cross(Vector3::Up(), forward).GetNormalized();
    Vector3 up = Vector3::Cross(forward, right);

    for (int i = 0; i < pelletCount; i++)
    {
        float yaw = GameMath::Random(-spreadAngle, spreadAngle);
        float pitch = GameMath::Random(-spreadAngle, spreadAngle);

        Matrix rotation =
            XMMatrixRotationAxis(up, XMConvertToRadians(yaw)) *
            XMMatrixRotationAxis(right, XMConvertToRadians(pitch));

        Vector3 shotDir = XMVector3TransformNormal(forward, rotation);

        Ray ray(pos, shotDir);

        for (Enemy* enemy : EnemyManager::Get()->GetAllEnemies())
        {
            if (!enemy->IsActive()) continue;

            RaycastHit hit;
            if (enemy->IsRayCollision(ray, &hit))
            {
                enemy->TakeDamage(damage);

                Vector3 knockbackDir = shotDir.GetNormalized();
                knockbackDir.y = 0.0f;
                knockbackDir.Normalize();

                float knockbackPower = 2.0f;
                enemy->Translate(knockbackDir * knockbackPower);
            }
        }
    }
}
