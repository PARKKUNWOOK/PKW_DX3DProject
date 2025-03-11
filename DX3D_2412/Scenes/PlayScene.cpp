#include "Framework.h"
#include "PlayScene.h"

PlayScene::PlayScene()
{
    srand(static_cast<unsigned int>(time(nullptr)));

	MapManager::Get()->Load("Resources/TextData/Pacman.map");

	player = new Player();
	player->SetLocalPosition(1, 5, 1);

    enemyPool = new PoolingManager<Enemy>();
    enemyPool->Create(maxEnemies);

    SpawnEnemies(spawnCount);
}

PlayScene::~PlayScene()
{
	MapManager::Delete();

	delete player;
    delete enemyPool;
}

void PlayScene::Update()
{
    UIManager::Get()->Update();

    if (player->IsGameOver()) return;

	MapManager::Get()->Update();
	player->Update();

    int aliveEnemies = 0;

    for (Enemy* enemy : enemyPool->GetAllActive())
    {
        enemy->Update();
        if (enemy->IsActive())
        {
            aliveEnemies++;
        }
    }

    //if (aliveEnemies == 0)
    //{
    //    spawnCount;
    //    SpawnEnemies(spawnCount);
    //}
}

void PlayScene::PreRender()
{
}

void PlayScene::Render()
{
    MapManager::Get()->Render();

	player->Render();

    for (Enemy* enemy : enemyPool->GetAllActive())
    {
        enemy->Render();
    }
}

void PlayScene::PostRender()
{
    for (Enemy* enemy : enemyPool->GetAllActive())
    {
        enemy->PostRender();
    }
}

void PlayScene::GUIRender()
{
    for (Enemy* enemy : enemyPool->GetAllActive())
    {
        enemy->Edit();
    }
}

void PlayScene::SpawnEnemies(int count)
{
    int mapSize = 20;
    float spawnY = 1.5f;
    float minDistance = 2.0f;

    for (int i = 0; i < count; i++)
    {
        Enemy* enemy = enemyPool->Pop();
        if (!enemy) return;

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

            for (Enemy* other : enemyPool->GetAllActive())
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
            enemy->SetLocalPosition(x, spawnY, z);
            enemy->SetTarget(player);
            enemy->SetActive(true);
        }
    }
}
