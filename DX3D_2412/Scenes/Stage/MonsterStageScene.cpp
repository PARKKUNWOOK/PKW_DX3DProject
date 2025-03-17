#include "Framework.h"
#include "MonsterStageScene.h"

MonsterStageScene::MonsterStageScene()
{
    srand(static_cast<unsigned int>(time(nullptr)));

	MapManager::Get()->Load("Resources/TextData/Pacman.map");

	player = new Player();
	player->SetLocalPosition(5, 5, 1);

    EnemyManager::Get()->SpawnEnemies(1, player);
}

MonsterStageScene::~MonsterStageScene()
{
	MapManager::Delete();

	delete player;

    EnemyManager::Delete();
}

void MonsterStageScene::Update()
{
    UIManager::Get()->Update();

    if (player->IsGameOver()) return;

	MapManager::Get()->Update();
	player->Update();
    EnemyManager::Get()->Update();

    if (EnemyManager::Get()->IsAllDead())
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
    EnemyManager::Get()->Render();
}

void MonsterStageScene::PostRender()
{
}

void MonsterStageScene::GUIRender()
{
}
