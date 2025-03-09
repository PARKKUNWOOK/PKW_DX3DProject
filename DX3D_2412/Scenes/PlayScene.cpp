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

    SpawnEnemies();
}

PlayScene::~PlayScene()
{
	MapManager::Delete();

	delete player;
    delete enemyPool;
}

void PlayScene::Update()
{
	MapManager::Get()->Update();

	player->Update();

    for (Enemy* enemy : enemyPool->GetAllActive())
    {
        enemy->Update();

        if (enemyPool->GetAllActive().size() < spawnCount)
        {
            SpawnEnemies();
        }
    }
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
}

void PlayScene::SpawnEnemies()
{
    int mapSize = 20;
    float spawnY = 1.5f;

    for (int i = 0; i < spawnCount; i++)
    {
        Enemy* enemy = enemyPool->Pop();
        if (!enemy) return;

        int side = rand() % 4;
        float x, z;

        switch (side)
        {
        case 0:
            x = 10;
            z = static_cast<float>(rand() % mapSize);
            break;
        case 1:
            x = static_cast<float>(mapSize - 1);
            z = static_cast<float>(rand() % mapSize);
            break;
        case 2:
            x = static_cast<float>(rand() % mapSize);
            z = 10;
            break;
        case 3:
            x = static_cast<float>(rand() % mapSize);
            z = static_cast<float>(mapSize - 1);
            break;
        }

        enemy->SetLocalPosition(x, spawnY, z);
        enemy->SetTarget(player);
        enemy->SetActive(true);
    }
}
