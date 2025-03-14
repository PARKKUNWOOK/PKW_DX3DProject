#include "Framework.h"
#include "MonsterStageScene.h"

MonsterStageScene::MonsterStageScene()
{
    srand(static_cast<unsigned int>(time(nullptr)));

	MapManager::Get()->Load("Resources/TextData/Pacman.map");

	player = new Player();
	player->SetLocalPosition(1, 5, 1);

    assaultEnemyPool = new PoolingManager<AssaultEnemy>();
    assaultEnemyPool->Create(maxEnemies);

    throwerEnemyPool = new PoolingManager<ThrowerEnemy>();
    throwerEnemyPool->Create(maxEnemies);

    SpawnEnemies(spawnCount);
}

MonsterStageScene::~MonsterStageScene()
{
	MapManager::Delete();

	delete player;
    delete assaultEnemyPool;
    delete throwerEnemyPool;
}

void MonsterStageScene::Update()
{
    UIManager::Get()->Update();

    if (player->IsGameOver()) return;

	MapManager::Get()->Update();
	player->Update();

    int aliveEnemies = 0;

    for (AssaultEnemy* assaultEnemy : assaultEnemyPool->GetAllActive())
    {
        assaultEnemy->Update();
        if (assaultEnemy->IsActive())
        {
            aliveEnemies++;
        }
    }

    for (ThrowerEnemy* throwerEnemy : throwerEnemyPool->GetAllActive())
    {
        throwerEnemy->Update();
        if (throwerEnemy->IsActive())
        {
            aliveEnemies++;
        }
    }

    if (aliveEnemies == 0)
    {
        MapManager::Get()->OpenAllDoorsZ();
    }
}

void MonsterStageScene::PreRender()
{
}

void MonsterStageScene::Render()
{
    MapManager::Get()->Render();

	player->Render();

    for (AssaultEnemy* assaultEnemy : assaultEnemyPool->GetAllActive())
    {
        assaultEnemy->Render();
    }

    for (ThrowerEnemy* throwerEnemy : throwerEnemyPool->GetAllActive())
    {
        throwerEnemy->Render();
    }
}

void MonsterStageScene::PostRender()
{
}

void MonsterStageScene::GUIRender()
{
    for (AssaultEnemy* assaultEnemy : assaultEnemyPool->GetAllActive())
    {
        assaultEnemy->Edit();
    }

    for (ThrowerEnemy* throwerEnemy : throwerEnemyPool->GetAllActive())
    {
        throwerEnemy->Edit();
    }
}

void MonsterStageScene::SpawnEnemies(int count)
{
    int mapSize = 20;
    float spawnY = 1.5f;
    float minDistance = 2.0f;

    for (int i = 0; i < count; i++)
    {
        AssaultEnemy* assaultEnemy = assaultEnemyPool->Pop();
        if (!assaultEnemy) return;

        ThrowerEnemy* throwerEnemy = throwerEnemyPool->Pop();
        if (!throwerEnemy) return;

        float x, z;
        bool positionValid = false;

        for (int attempt = 0; attempt < 10; attempt++)
        {
            int side = rand() % 4;

            switch (side)
            {
            case 0:
                x = 20;
                z = static_cast<float>(rand() % mapSize);
                break;
            case 1:
                x = static_cast<float>(mapSize - 40);
                z = static_cast<float>(rand() % mapSize);
                break;
            case 2:
                x = static_cast<float>(rand() % mapSize);
                z = 20;
                break;
            case 3:
                x = static_cast<float>(rand() % mapSize);
                z = static_cast<float>(mapSize - 40);
                break;
            }

            positionValid = true;

            for (AssaultEnemy* other : assaultEnemyPool->GetAllActive())
            {
                if (other->IsActive())
                {
                    float distance = Vector3::Distance(Vector3(x, spawnY, z), other->GetLocalPosition());
                    if (distance < minDistance)
                    {
                        positionValid = false;
                        break;
                    }
                }
            }

            for (ThrowerEnemy* other : throwerEnemyPool->GetAllActive())
            {
                if (other->IsActive())
                {
                    float distance = Vector3::Distance(Vector3(x, spawnY, z), other->GetLocalPosition());
                    if (distance < minDistance)
                    {
                        positionValid = false;
                        break;
                    }
                }
            }

            if (positionValid) break;
        }

        if (positionValid)
        {
            assaultEnemy->SetLocalPosition(x, spawnY, z);
            assaultEnemy->SetTarget(player);
            assaultEnemy->SetActive(true);

            throwerEnemy->SetTarget(player);
            throwerEnemy->SetLocalPosition(x, spawnY, z);
            throwerEnemy->SetActive(true);
        }
    }
}
