#include "Framework.h"
#include "MonsterRoomScene.h"

MonsterRoomScene::MonsterRoomScene()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

MonsterRoomScene::~MonsterRoomScene()
{
    EnemyManager::Delete();
}

void MonsterRoomScene::Update()
{
    UIManager::Get()->Update();
	MapManager::Get()->Update();
    PlayerManager::Get()->Update();
    EnemyManager::Get()->Update();

    CheckSceneTransition();

    if (!EnemyManager::Get()->IsAllDead())
    {
        MapManager::Get()->CloseAllDoorsX();
        MapManager::Get()->CloseAllDoorsZ();
    }
    else
    {
        MapManager::Get()->OpenDoorX(1);
        MapManager::Get()->OpenDoorZ(0);
        MapManager::Get()->CloseDoorX(0);
        MapManager::Get()->CloseDoorZ(1);
        MapManager::Get()->SetMonsterStageCleared(true);
    }
}

void MonsterRoomScene::PreRender()
{
}

void MonsterRoomScene::Render()
{
    MapManager::Get()->Render();
    PlayerManager::Get()->Render();
    EnemyManager::Get()->Render();
}

void MonsterRoomScene::PostRender()
{
}

void MonsterRoomScene::GUIRender()
{
    PlayerManager::Get()->Edit();
    EnemyManager::Get()->Edit();
}

void MonsterRoomScene::CheckSceneTransition()
{
    Player* player = Player::Get();

    bool touchingPortal = false;

    const vector<Portal*>& portals = MapManager::Get()->GetPortals();

    for (int i = 0; i < portals.size(); i++)
    {
        Portal* portal = portals[i];

        if (player->GetCollider()->IsBoxCollision(portal->GetCollider()))
        {
            if (!portal->IsActive())
                continue;

            touchingPortal = true;

            if (!isTouchingPortal)
            {
                // 포탈이 0번이면 X축 1번 문 -> StartRoom
                if (i == 0)
                {
                    SCENE->Remove("Monster");
                    SCENE->Add("Start");

                    Vector3 target = MapManager::Get()->GetDoorX(0)->GetLocalPosition();
                    Player::Get()->SetLocalPosition(target + Vector3(0, 0, 3));
                }
                // 포탈이 1번이면 Z축 0번 문 -> StoreRoom
                else if (i == 1)
                {
                    SCENE->Remove("Monster");
                    SCENE->Add("Store");

                    Vector3 target = MapManager::Get()->GetDoorZ(1)->GetLocalPosition();
                    Player::Get()->SetLocalPosition(target + Vector3(-3, 0, 0));
                }
            }
            break;
        }
    }

    isTouchingPortal = touchingPortal;
}
