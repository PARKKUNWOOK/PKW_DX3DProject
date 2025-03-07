#include "Framework.h"
#include "PlayScene.h"

PlayScene::PlayScene()
{
	MapManager::Get()->Load("Resources/TextData/Pacman.map");
}

PlayScene::~PlayScene()
{
	MapManager::Delete();
}

void PlayScene::Update()
{
	MapManager::Get()->Update();
}

void PlayScene::PreRender()
{
}

void PlayScene::Render()
{
    MapManager::Get()->Render();
}

void PlayScene::PostRender()
{
}

void PlayScene::GUIRender()
{
}
