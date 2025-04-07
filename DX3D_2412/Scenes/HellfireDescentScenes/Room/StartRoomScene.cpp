#include "Framework.h"
#include "StartRoomScene.h"

StartRoomScene::StartRoomScene()
{
	MapManager::Get()->Load("Resources/TextData/Pacman.map");
    UIManager::Get();
}

StartRoomScene::~StartRoomScene()
{
    UIManager::Delete();
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
    EffectManager::Get()->Update();
}

void StartRoomScene::PreRender()
{
}

void StartRoomScene::Render()
{
	MapManager::Get()->Render();

	PlayerManager::Get()->Render();
    EffectManager::Get()->Render();
}

void StartRoomScene::PostRender()
{
    UIManager::Get()->Render();
}

void StartRoomScene::GUIRender()
{
    PlayerManager::Get()->Edit();
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

                //if (!Audio::Get()->IsPlaySound("BGM"))
                //{
                //    if (!Audio::Get()->IsExist("BGM"))
                //        Audio::Get()->Add("BGM", "Resources/Sounds/HellfireDescentSound/At_Doom's_Gate.mp3", true);
                //
                //    Audio::Get()->Play("BGM");
                //}

                Vector3 target = MapManager::Get()->GetDoorX(1)->GetLocalPosition();
                Player::Get()->SetLocalPosition(target + Vector3(0, 0, -3));
            }
            break;
        }
    }

    isTouchingPortal = touchingPortal;
}
