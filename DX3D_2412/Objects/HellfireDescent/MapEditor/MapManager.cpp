#include "Framework.h"

MapManager::MapManager()
{
	FOR(PORTAL_POOL_SIZE)
	{
		Portal* portal = new Portal({ 1, 1, 1 });
		portal->SetActive(false);
		portals.push_back(portal);
	}
}

MapManager::~MapManager()
{
}

void MapManager::Update()
{
	for (Cube* door : loweringDoors)
	{
		Vector3 position = door->GetLocalPosition();
		position.y -= 2.0f * DELTA;

		if (position.y < -10.0f) // 특정 위치 아래로 내려가면 제거
		{
			door->SetActive(false);
		}
		else
		{
			door->SetLocalPosition(position);
		}
	}

	for (Portal* portal : portals)
	{
		portal->Update();
	}
}

void MapManager::Render()
{
	floor->Render();

	for (Cube* wall : walls)
		wall->Render();
	for (Cube* door : doorxs)
		door->Render();
	for (Cube* door : doorzs)
		door->Render();
	for (Portal* portal : portals)
		portal->Render();
}

void MapManager::Load(string file)
{
	BinaryReader* reader = new BinaryReader("Resources/TextData/Pacman.map");

	POINT mapSize = reader->Data<POINT>();
	POINT mapTiling = reader->Data<POINT>();

	mapWidth = mapSize.x;
	mapHeight = mapSize.y;

	floor = new Cube(Vector3(mapSize.x, 1.0f, mapSize.y), mapTiling);
	floor->GetMaterial()->Load("Resources/Materials/PacmanMap.mat");

	int size = mapSize.x * mapSize.y;

	walls.reserve(size);
	doorxs.reserve(size);
	doorzs.reserve(size);

	int* types = new int[size];
	reader->Byte((void**)&types, sizeof(int) * size);

	FOR(size)
	{
		switch (types[i])
		{
		case 1:
		{
			Vector2 coord = Vector2(i % mapSize.x, i / mapSize.x);

			Vector3 startPos = Vector3(mapSize.x * -0.5f, 0.0f, mapSize.y * -0.5f);
			Vector3 pos = startPos + Vector3(coord.x + 0.5f, 8.0f, coord.y + 0.5f);

			Cube* wall = new Cube({ 1, 15, 1 });
			wall->SetLocalPosition(pos);
			wall->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Wall.png");
			wall->Update();

			walls.push_back(wall);
		}
		break;
		case 2:
		{
			Vector2 coord = Vector2(i % mapSize.x, i / mapSize.x);

			Vector3 startPos = Vector3(mapSize.x * -0.5f, 0.0f, mapSize.y * -0.5f);
			Vector3 pos = startPos + Vector3(coord.x + 0.5f, 8.0f, coord.y + 0.5f);

			Cube* doorx = new Cube({ 7, 15, 1 });
			doorx->SetLocalPosition(pos);
			doorx->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Wall.png");
			doorx->Update();

			doorxs.push_back(doorx);
		}
		break;
		case 3:
		{
			Vector2 coord = Vector2(i % mapSize.x, i / mapSize.x);

			Vector3 startPos = Vector3(mapSize.x * -0.5f, 0.0f, mapSize.y * -0.5f);
			Vector3 pos = startPos + Vector3(coord.x + 0.5f, 8.0f, coord.y + 0.5f);

			Cube* doorz = new Cube({ 1, 15, 7 });
			doorz->SetLocalPosition(pos);
			doorz->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Wall.png");
			doorz->Update();

			doorzs.push_back(doorz);
		}
		break;
		}
	}

	delete[] types;

	delete reader;
}

float MapManager::GetHeight(const Vector3& pos) const
{
	Ray ray(pos, Vector3::Down());
	RaycastHit hit;

	float maxHeight = 0.0f;

	if (floor->GetCollider()->IsRayCollision(ray, &hit))
	{
		if (hit.point.y > maxHeight)
			maxHeight = hit.point.y;
	}

	return maxHeight;
}

Vector3 MapManager::ResolveCollisions(Player* player, float& totalOverlab)
{
	SphereCollider* playerCollider = player;
	Vector3 playerPos = player->GetLocalPosition();
	Vector3 totalPushDirection = Vector3::Zero();
	RaycastHit hit;

	for (Cube* wall : walls)
	{
		BoxCollider* wallCollider = wall->GetCollider();
		if (wallCollider->PushSphere(playerCollider, &hit))
		{
			totalPushDirection += hit.normal * hit.distance;
			totalOverlab += hit.distance;
		}
	}

	for (int i = 0; i < doorxs.size(); i++)
	{
		if (IsDoorXOpen(i)) continue;

		BoxCollider* doorCollider = doorxs[i]->GetCollider();
		if (doorCollider->PushSphere(playerCollider, &hit))
		{
			totalPushDirection += hit.normal * hit.distance;
			totalOverlab += hit.distance;
		}
	}

	for (int i = 0; i < doorzs.size(); i++)
	{
		if (IsDoorZOpen(i)) continue;

		BoxCollider* doorCollider = doorzs[i]->GetCollider();
		if (doorCollider->PushSphere(playerCollider, &hit))
		{
			totalPushDirection += hit.normal * hit.distance;
			totalOverlab += hit.distance;
		}
	}

	return totalPushDirection.GetNormalized();
}

void MapManager::OpenDoorX(int index)
{
	if (index >= 0 && index < doorxs.size())
	{
		doorxs[index]->SetActive(false);
		UpdatePortals();
	}
}

void MapManager::OpenDoorZ(int index)
{
	if (index >= 0 && index < doorzs.size())
	{
		doorzs[index]->SetActive(false);
		UpdatePortals();
	}
}

void MapManager::OpenAllDoorsX()
{
	for (Cube* door : doorxs)
	{
		door->SetActive(false);
	}
	UpdatePortals();
}

void MapManager::OpenAllDoorsZ()
{
	for (Cube* door : doorzs)
	{
		door->SetActive(false);
	}
	UpdatePortals();
}

void MapManager::CloseDoorX(int index)
{
	if (index >= 0 && index < doorxs.size())
	{
		doorxs[index]->SetActive(true);
		UpdatePortals();
	}
}

void MapManager::CloseDoorZ(int index)
{
	if (index >= 0 && index < doorzs.size())
	{
		doorzs[index]->SetActive(true);
		UpdatePortals();
	}
}

void MapManager::CloseAllDoorsX()
{
	for (Cube* door : doorxs)
	{
		door->SetActive(true);
	}
	UpdatePortals();
}

void MapManager::CloseAllDoorsZ()
{
	for (Cube* door : doorzs)
	{
		door->SetActive(true);
	}
	UpdatePortals();
}

bool MapManager::IsDoorXOpen(int index)
{
	if (index >= 0 && index < doorxs.size())
		return !doorxs[index]->IsActive();

	return false;
}

bool MapManager::IsDoorZOpen(int index)
{
	if (index >= 0 && index < doorzs.size())
		return !doorzs[index]->IsActive();

	return false;
}

void MapManager::UpdatePortals()
{
	for (Portal* portal : portals)
		portal->SetActive(false);

	int portalIndex = 0;

	// X축 문이 열려 있으면 포탈 생성
	for (int i = 0; i < doorxs.size(); i++)
	{
		if (IsDoorXOpen(i) && portalIndex < portals.size())
		{
			portals[portalIndex]->SetLocalPosition(doorxs[i]->GetLocalPosition());
			portals[portalIndex]->SetLocalScale(Vector3(7, 15, 0.1f));
			portals[portalIndex]->SetActive(true);
			portalIndex++;
		}
	}

	// Z축 문이 열려 있으면 포탈 생성
	for (int i = 0; i < doorzs.size(); i++)
	{
		if (IsDoorZOpen(i) && portalIndex < portals.size())
		{
			portals[portalIndex]->SetLocalPosition(doorzs[i]->GetLocalPosition());
			portals[portalIndex]->SetLocalScale(Vector3(0.1f, 15, 7));
			portals[portalIndex]->SetActive(true);
			portalIndex++;
		}
	}
}
