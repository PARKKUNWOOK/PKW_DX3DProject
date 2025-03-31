#include "Framework.h"

CombatShotgun::CombatShotgun()
{
}

CombatShotgun::~CombatShotgun()
{
}

void CombatShotgun::Fire(Vector3 pos, Vector3 dir, bool isPlayer)
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

                if (enemy->GetCurrentHealth() == 0)
                {
                    if (dynamic_cast<AssaultEnemy*>(enemy))
                    {
                        Audio::Get()->Add("AssaultDeath", "Resources/Sounds/HellfireDescentSound/AssaultEnemyDeath.ogg");
                        Audio::Get()->Play("AssaultDeath", 3.0f);
                    }
                    else if (dynamic_cast<ThrowerEnemy*>(enemy))
                    {
                        Audio::Get()->Add("ThrowerDeath", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyDeath.ogg");
                        Audio::Get()->Play("ThrowerDeath", 3.0f);
                    }
                }
                else
                {
                    if (dynamic_cast<AssaultEnemy*>(enemy))
                    {
                        Audio::Get()->Add("AssaultDamage", "Resources/Sounds/HellfireDescentSound/AssaultEnemyDamage.ogg");
                        Audio::Get()->Play("AssaultDamage", 3.0f);
                    }
                    else if (dynamic_cast<ThrowerEnemy*>(enemy))
                    {
                        Audio::Get()->Add("ThrowerDamage", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyDamage.ogg");
                        Audio::Get()->Play("ThrowerDamage", 3.0f);
                    }
                }
            }
        }
    }
}
