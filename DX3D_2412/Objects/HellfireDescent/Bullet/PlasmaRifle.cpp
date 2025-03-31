#include "Framework.h"

PlasmaRifle::PlasmaRifle()
{
}

PlasmaRifle::~PlasmaRifle()
{
}

void PlasmaRifle::Fire(Vector3 pos, Vector3 dir, bool isPlayer)
{
	unordered_set<Enemy*> hitEnemies;
	FireChain(pos, dir.GetNormalized(), isPlayer, 0, hitEnemies);
}

void PlasmaRifle::FireChain(Vector3 origin, Vector3 direction, bool isPlayer, int chainCount, unordered_set<Enemy*>& hitEnemies)
{
    if (chainCount >= maxChainCount)
        return;

    Ray ray(origin, direction);

    Enemy* closestEnemy = nullptr;
    RaycastHit closestHit;
    float minDistance = FLT_MAX;

    for (Enemy* enemy : EnemyManager::Get()->GetAllEnemies())
    {
        if (!enemy->IsActive()) continue;
        if (hitEnemies.find(enemy) != hitEnemies.end()) continue;

        RaycastHit hit;
        if (enemy->IsRayCollision(ray, &hit))
        {
            float dist = (hit.point - origin).Magnitude();
            if (dist < minDistance)
            {
                minDistance = dist;
                closestHit = hit;
                closestEnemy = enemy;
            }
        }
    }

    if (closestEnemy)
    {
        closestEnemy->TakeDamage(damage);

        if (closestEnemy->GetCurrentHealth() == 0)
        {
            if (dynamic_cast<AssaultEnemy*>(closestEnemy))
            {
                Audio::Get()->Add("AssaultDeath", "Resources/Sounds/HellfireDescentSound/AssaultEnemyDeath.ogg");
                Audio::Get()->Play("AssaultDeath", 3.0f);
            }
            else if (dynamic_cast<ThrowerEnemy*>(closestEnemy))
            {
                Audio::Get()->Add("ThrowerDeath", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyDeath.ogg");
                Audio::Get()->Play("ThrowerDeath", 3.0f);
            }
        }
        else
        {
            if (dynamic_cast<AssaultEnemy*>(closestEnemy))
            {
                Audio::Get()->Add("AssaultDamage", "Resources/Sounds/HellfireDescentSound/AssaultEnemyDamage.ogg");
                Audio::Get()->Play("AssaultDamage", 3.0f);
            }
            else if (dynamic_cast<ThrowerEnemy*>(closestEnemy))
            {
                Audio::Get()->Add("ThrowerDamage", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyDamage.ogg");
                Audio::Get()->Play("ThrowerDamage", 3.0f);
            }
        }

        hitEnemies.insert(closestEnemy);

        Vector3 chainOrigin = closestEnemy->GetGlobalPosition();

        for (Enemy* enemy : EnemyManager::Get()->GetAllEnemies())
        {
            if (!enemy->IsActive()) continue;
            if (hitEnemies.find(enemy) != hitEnemies.end()) continue;
            if (enemy == closestEnemy) continue;

            float dist = (enemy->GetGlobalPosition() - chainOrigin).Magnitude();
            if (dist <= chainRadius)
            {
                Vector3 nextDir = (enemy->GetGlobalPosition() - chainOrigin).GetNormalized();
                FireChain(chainOrigin, nextDir, isPlayer, chainCount + 1, hitEnemies);
                break;
            }
        }
    }
}
