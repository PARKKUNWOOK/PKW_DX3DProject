#include "Framework.h"
#include "MonsterStageScene.h"

MonsterStageScene::MonsterStageScene()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

MonsterStageScene::~MonsterStageScene()
{
    EnemyManager::Delete();
}

void MonsterStageScene::Update()
{
    UIManager::Get()->Update();

	MapManager::Get()->Update();
    PlayerManager::Get()->Update();
    EnemyManager::Get()->Update();

    if (!EnemyManager::Get()->IsAllDead())
    {
        MapManager::Get()->AllCloseDoors();
    }


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
    PlayerManager::Get()->Render();
    EnemyManager::Get()->Render();
}

void MonsterStageScene::PostRender()
{
}

void MonsterStageScene::GUIRender()
{
    EnemyManager::Get()->Edit();
}
