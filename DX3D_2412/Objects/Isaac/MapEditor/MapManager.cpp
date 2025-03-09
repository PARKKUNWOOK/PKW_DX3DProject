#include "Framework.h"

MapManager::MapManager()
{	
}

MapManager::~MapManager()
{
}

void MapManager::Update()
{
}

void MapManager::Render()
{
	floor->Render();

	for (Cube* wall : walls)
		wall->Render();
	for (Cube* startPosition : startPositions)
		startPosition->Render();
}

void MapManager::Load(string file)
{
	BinaryReader* reader = new BinaryReader("Resources/TextData/Pacman.map");

	POINT mapSize = reader->Data<POINT>();
	POINT mapTiling = reader->Data<POINT>();

	floor = new Cube(Vector3(mapSize.x, 1.0f, mapSize.y), mapTiling);
	floor->GetMaterial()->Load("Resources/Materials/PacmanMap.mat");

	int size = mapSize.x * mapSize.y;

	walls.reserve(size);

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
			Vector3 pos = startPos + Vector3(coord.x + 0.5f, 0.5f, coord.y + 0.5f);

			Cube* startPosition = new Cube({ 1, 0.1f, 1 });
			startPosition->SetLocalPosition(pos);
			startPosition->GetMaterial()->GetData()->diffuse = { 0, 1, 1, 1 };
			startPosition->Update();

			startPositions.push_back(startPosition);
		}
		break;
		}
	}

	delete[] types;

	delete reader;
}
