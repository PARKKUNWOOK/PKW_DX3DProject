#include "Framework.h"

MapManager::MapManager()
{	
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

void MapManager::ResolveCollisions(Player* player)
{
	SphereCollider* playerCollider = player;
	Vector3 playerPos = player->GetLocalPosition();

	for (Cube* wall : walls)
	{
		BoxCollider* wallCollider = wall->GetCollider();
		if (wallCollider->IsSphereCollision(playerCollider))
		{
			Vector3 pushDirection = playerPos - wall->GetLocalPosition();
			pushDirection.Normalize();

			player->SetLocalPosition(playerPos + pushDirection * 0.1f);
			wall->SetLocalPosition(wall->GetLocalPosition() - pushDirection * 0.1f);
		}
	}

	for (Cube* door : doorxs)
	{
		if (!door->IsActive()) return;

		BoxCollider* doorCollider = door->GetCollider();
		if (doorCollider->IsSphereCollision(playerCollider))
		{
			Vector3 pushDirection = playerPos - door->GetLocalPosition();
			pushDirection.Normalize();

			player->SetLocalPosition(playerPos + pushDirection * 0.1f);
			door->SetLocalPosition(door->GetLocalPosition() - pushDirection * 0.1f);
		}
	}

	for (Cube* door : doorzs)
	{
		if (!door->IsActive()) return;

		BoxCollider* doorCollider = door->GetCollider();
		if (doorCollider->IsSphereCollision(playerCollider))
		{
			Vector3 pushDirection = playerPos - door->GetLocalPosition();
			pushDirection.Normalize();

			player->SetLocalPosition(playerPos + pushDirection * 0.1f);
			door->SetLocalPosition(door->GetLocalPosition() - pushDirection * 0.1f);
		}
	}
}

void MapManager::AllCloseDoors()
{
	for (Cube* door : doorxs)
	{
		door->SetActive(true);
	}

	for (Cube* door : doorzs)
	{
		door->SetActive(true);
	}

	loweringDoors.clear();
}

void MapManager::OpenDoorX(int index)
{
	if (index >= 0 && index < doorxs.size())
	{
		loweringDoors.push_back(doorxs[index]);
	}
}

void MapManager::OpenDoorZ(int index)
{
	if (index >= 0 && index < doorzs.size())
	{
		loweringDoors.push_back(doorzs[index]);
	}
}

void MapManager::OpenAllDoorsX()
{
	for (Cube* door : doorxs)
	{
		loweringDoors.push_back(door);
	}
}

void MapManager::OpenAllDoorsZ()
{
	for (Cube* door : doorzs)
	{
		loweringDoors.push_back(door);
	}
}

