#include "Framework.h"

PlasmaRifle::PlasmaRifle()
{
    weaponType = WeaponType::PlasmaRifle;
    damage = 5;
}

PlasmaRifle::~PlasmaRifle()
{
}

void PlasmaRifle::Fire(Vector3 pos, Vector3 dir)
{
	unordered_set<Enemy*> hitEnemies;
	FireChain(pos, dir.GetNormalized(), 0, hitEnemies);
}

void PlasmaRifle::FireChain(Vector3 origin, Vector3 direction, int chainCount, unordered_set<Enemy*>& hitEnemies)
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
                FireChain(chainOrigin, nextDir, chainCount + 1, hitEnemies);
                break;
            }
        }
    }
}
