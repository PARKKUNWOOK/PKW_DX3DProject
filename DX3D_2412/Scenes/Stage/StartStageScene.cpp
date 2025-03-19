#include "Framework.h"
#include "StartStageScene.h"

StartStageScene::StartStageScene()
{
	MapManager::Get()->Load("Resources/TextData/Pacman.map");
}

StartStageScene::~StartStageScene()
{
}

void StartStageScene::Update()
{
	UIManager::Get()->Update();

	MapManager::Get()->Update();

	PlayerManager::Get()->Update();

	MapManager::Get()->OpenDoorX(0);
}

void StartStageScene::PreRender()
{
}

void StartStageScene::Render()
{
	MapManager::Get()->Render();

	PlayerManager::Get()->Render();
}

void StartStageScene::PostRender()
{
}

void StartStageScene::GUIRender()
{
}
