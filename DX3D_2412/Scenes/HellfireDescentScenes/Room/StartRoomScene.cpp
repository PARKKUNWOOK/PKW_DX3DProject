#include "Framework.h"
#include "StartRoomScene.h"

StartRoomScene::StartRoomScene()
{
	MapManager::Get()->Load("Resources/TextData/Pacman.map");
}

StartRoomScene::~StartRoomScene()
{
}

void StartRoomScene::Update()
{
	UIManager::Get()->Update();
	MapManager::Get()->Update();
	PlayerManager::Get()->Update();

    CheckSceneTransition();

	MapManager::Get()->OpenDoorX(0);

	MapManager::Get()->CloseAllDoorsZ();
	MapManager::Get()->CloseDoorX(1);
}

void StartRoomScene::PreRender()
{
}

void StartRoomScene::Render()
{
	MapManager::Get()->Render();

	PlayerManager::Get()->Render();
}

void StartRoomScene::PostRender()
{
}

void StartRoomScene::GUIRender()
{
}

void StartRoomScene::CheckSceneTransition()
{
    Player* player = Player::Get();

    bool touchingPortal = false;

    for (Portal* portal : MapManager::Get()->GetPortals())
    {
        if (player->GetCollider()->IsBoxCollision(portal->GetCollider()))
        {
            if (!portal->IsActive())
                continue;

            touchingPortal = true;

            if (!isTouchingPortal)
            {
                SCENE->Remove("Start");
                SCENE->Add("Monster");

                Vector3 target = MapManager::Get()->GetDoorX(1)->GetLocalPosition();
                Player::Get()->SetLocalPosition(target + Vector3(0, 0, -3));
            }
            break;
        }
    }

    isTouchingPortal = touchingPortal;
}
