#include "Framework.h"
#include "StartStageScene.h"

StartStageScene::StartStageScene()
{
	MapManager::Get()->Load("Resources/TextData/Pacman.map");
	player = new Player();
	player->SetLocalPosition(1, 5, 1);
}

StartStageScene::~StartStageScene()
{
	MapManager::Delete();

	delete player;
}

void StartStageScene::Update()
{
	UIManager::Get()->Update();

	MapManager::Get()->Update();

	player->Update();
}

void StartStageScene::PreRender()
{
}

void StartStageScene::Render()
{
	MapManager::Get()->Render();

	player->Render();
}

void StartStageScene::PostRender()
{
}

void StartStageScene::GUIRender()
{
}
