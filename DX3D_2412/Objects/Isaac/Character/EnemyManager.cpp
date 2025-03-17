#include "Framework.h"

EnemyManager::EnemyManager()
{
    for (int i = 0; i < maxEnemies; i++)
    {
        assaultEnemies.push_back(new AssaultEnemy());
        throwerEnemies.push_back(new ThrowerEnemy());
    }
}

EnemyManager::~EnemyManager()
{
    for (AssaultEnemy* enemy : assaultEnemies)
        delete enemy;

    for (ThrowerEnemy* enemy : throwerEnemies)
        delete enemy;
}

void EnemyManager::Update()
{
    for (list<AssaultEnemy*>::iterator it = assaultEnemies.begin(); it != assaultEnemies.end();)
    {
        AssaultEnemy* enemy = *it;
        if (enemy->IsActive())
        {
            enemy->Update();
            ++it;
        }
        else
        {
            it = assaultEnemies.erase(it);
        }
    }

    for (list<ThrowerEnemy*>::iterator it = throwerEnemies.begin(); it != throwerEnemies.end();)
    {
        ThrowerEnemy* enemy = *it;
        if (enemy->IsActive())
        {
            enemy->Update();
            ++it;
        }
        else
        {
            it = throwerEnemies.erase(it);
        }
    }
}

void EnemyManager::Render()
{
    for (AssaultEnemy* enemy : assaultEnemies)
    {
        if (enemy->IsActive())
            enemy->Render();
    }

    for (ThrowerEnemy* enemy : throwerEnemies)
    {
        if (enemy->IsActive())
            enemy->Render();
    }
}

void EnemyManager::Edit()
{
    for (AssaultEnemy* enemy : assaultEnemies)
    {
        enemy->Edit();
    }

    for (ThrowerEnemy* enemy : throwerEnemies)
    {
        enemy->Edit();
    }
}

void EnemyManager::SpawnEnemies(int count, Player* player)
{
    for (int i = 0; i < count; i++)
    {
        Vector3 spawnPos = GetRandomSpawnPosition();

        for (AssaultEnemy* enemy : assaultEnemies)
        {
            if (!enemy->IsActive())
            {
                enemy->SetLocalPosition(spawnPos);
                enemy->SetTarget(player);
                enemy->SetActive(true);
                break;
            }
        }

        for (ThrowerEnemy* enemy : throwerEnemies)
        {
            if (!enemy->IsActive())
            {
                enemy->SetLocalPosition(spawnPos);
                enemy->SetTarget(player);
                enemy->SetActive(true);
                break;
            }
        }
    }
}

bool EnemyManager::IsAllDead()
{
    return assaultEnemies.empty() && throwerEnemies.empty();
}

Vector3 EnemyManager::GetRandomSpawnPosition()
{
    Vector2 mapSize = MapManager::Get()->GetMapSize();
    float halfWidth = mapSize.x * 0.5f;
    float halfHeight = mapSize.y * 0.5f;

    int side = rand() % 4;
    //int side = 3;
    float x = 0, z = 0;

    switch (side)
    {
    case 0: //상단
        x = GameMath::Random(-halfWidth, +halfWidth);
        z = halfHeight;
        break;
    case 1: //하단
        x = GameMath::Random(-halfWidth, +halfWidth);
        z = -halfHeight;
        break;
    case 2: //좌측
        x = -halfWidth;
        z = GameMath::Random(-halfHeight, +halfHeight);
        break;
    case 3: //우측
        x = +halfWidth;
        z = GameMath::Random(-halfHeight, +halfHeight);
        break;
    }

    float groundY = MapManager::Get()->GetHeight(Vector3(x, 0, z));

    return Vector3(x, groundY + 1.5f, z);
}
