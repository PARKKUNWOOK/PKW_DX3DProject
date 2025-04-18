#include "Framework.h"
#include "StoreRoomScene.h"

StoreRoomScene::StoreRoomScene()
{
    UIManager::Get();
}

StoreRoomScene::~StoreRoomScene()
{
}

void StoreRoomScene::Update()
{
	UIManager::Get()->Update();
	MapManager::Get()->Update();
	PlayerManager::Get()->Update();

    CheckSceneTransition();

	MapManager::Get()->OpenDoorZ(1);
	MapManager::Get()->CloseDoorZ(0);
	MapManager::Get()->CloseAllDoorsX();
}

void StoreRoomScene::PreRender()
{
}

void StoreRoomScene::Render()
{
	MapManager::Get()->Render();
	PlayerManager::Get()->Render();
}

void StoreRoomScene::PostRender()
{
    UIManager::Get()->Render();
}

void StoreRoomScene::GUIRender()
{
    //PlayerManager::Get()->Edit();
}

void StoreRoomScene::CheckSceneTransition()
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
                SCENE->Remove("Store");
                SCENE->Add("Monster");

                Vector3 target = MapManager::Get()->GetDoorZ(0)->GetLocalPosition();
                Player::Get()->SetLocalPosition(target + Vector3(3, 0, 0));
            }
            break;
        }
    }

    isTouchingPortal = touchingPortal;
}
