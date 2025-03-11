#pragma once

class CubeMapEditor : public Cube
{
private:
	enum EditType
	{
		None, Wall, Door
	};

public:
	CubeMapEditor();
	~CubeMapEditor();

	void Update();
	void Render();
	void Edit();

private:
	void SetSelectTile();

	void CreateWall();
	void CreateDoor();
	void CreatePreview();

	void DeleteObject();

	void SetEdit();

	void UpdateMesh();

	void Save();
	void Load();

private:	
	POINT mapSize;
	POINT mapTiling = { 1, 1 };
	POINT selectTile;

	EditType editType = None;

	FloatValueBuffer* tileBuffer;

	list<GameObject*> objects;

	map<int, GameObject*> objectData;

	vector<EditType> types;

	map<EditType, GameObject*> previews;
};