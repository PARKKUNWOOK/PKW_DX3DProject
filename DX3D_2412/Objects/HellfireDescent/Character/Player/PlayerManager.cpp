#include "Framework.h"

PlayerManager::PlayerManager()
{
	player = new Player();
	player->SetLocalPosition(1, 5, 1);

	EnemyManager::Get()->SpawnEnemies(10, player);
}

PlayerManager::~PlayerManager()
{
	delete player;
}

void PlayerManager::Update()
{
	player->Update();
}

void PlayerManager::Render()
{
	player->Render();
}

void PlayerManager::PostRender()
{
}

void PlayerManager::Edit()
{
	player->Edit();
}
