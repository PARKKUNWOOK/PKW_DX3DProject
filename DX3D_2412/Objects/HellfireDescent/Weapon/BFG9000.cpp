#include "Framework.h"

//일반발사 : Damage 2
//차지공격 : 직격Damage 10,
//직격Damage 후 폭발범위공격 Damage5,
//총알이 날라가는중 주위의 적에게 지속Damage1


BFG9000::BFG9000(Transform* transform)
{
    weaponType = WeaponType::BFG9000;

    SetTag(transform->GetTag() + "_Collider");
    transform->SetParent(this);
    transform->SetTag("BFG9000_0");
    transform->Load();
}

BFG9000::~BFG9000()
{
}

void BFG9000::Update()
{
    if (!IsActive()) return;

    if (isCharged && exploded)
    {
        explosionTime -= DELTA;
        if (explosionTime <= 0.0f)
        {
            SetActive(false);
        }
        return;
    }

    lifeTime += DELTA;
    if (lifeTime > LIFE_TIME)
    {
        SetActive(false);
        return;
    }

    if (isCharged)
    {
        rayAttackTimer += DELTA;
        if (rayAttackTimer >= rayInterval)
        {
            rayAttackTimer = 0.0f;
            EmitRays();
        }
    }

    Translate(velocity * speed * DELTA);
    UpdateWorld();
}

void BFG9000::Render()
{
    Weapon::Render();
}

void BFG9000::Fire(Vector3 pos, Vector3 dir, bool charged)
{
    localPosition = pos;
    velocity = dir;
    lifeTime = 0.0f;
    exploded = false;
    radius = 0.9f;
    isCharged = charged;
    rayAttackTimer = 0.0f;

    direction = dir;
    direction.Normalize();

    float yaw = atan2(dir.x, dir.z);
    float pitch = -asin(dir.y);

    localRotation.x = pitch + XM_PIDIV2;
    localRotation.y = yaw;

    damage = isCharged ? 10 : 2;

    SetActive(true);
}

bool BFG9000::EnemyCollisionCheck(Enemy* enemy)
{
    if (!IsActive() || !enemy->IsActive()) return false;

    if (Intersects(enemy))
    {
        if (isCharged)
        {
            hitEnemy = enemy;
            Explode();
        }

        SetActive(false);
        return true;
    }

    return false;
}

void BFG9000::Explode()
{
    exploded = true;

    if (transform)
        transform->SetActive(false);

    radius = 999999999.0f;
    explosionTime = 0.1f;

    for (Enemy* enemy : EnemyManager::Get()->GetAllEnemies())
    {
        if (!enemy->IsActive()) continue;
        if (enemy == hitEnemy) continue;

        if (IsSphereCollision(enemy))
        {
            enemy->TakeDamage(5);
        }
    }
}

void BFG9000::EmitRays()
{
    Vector3 pos = GetGlobalPosition();
    float currentTime = Timer::Get()->GetTotalTime();

    for (int i = 0; i < 12; i++)
    {
        float angle = XM_2PI * i / 12.0f;
        Vector3 dir = Vector3(cos(angle), 0, sin(angle));
        Ray ray(pos, dir);

        for (Enemy* enemy : EnemyManager::Get()->GetAllEnemies())
        {
            if (!enemy->IsActive()) continue;

            RaycastHit hit;
            if (enemy->IsRayCollision(ray, &hit))
            {
                float& lastHit = lastRayHitTime[enemy];
                if (currentTime - lastHit >= rayInterval)
                {
                    enemy->TakeDamage(1);
                    lastHit = currentTime;
                }
            }
        }
    }
}
