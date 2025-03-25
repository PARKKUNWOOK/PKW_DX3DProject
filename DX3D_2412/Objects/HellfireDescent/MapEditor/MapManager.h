#pragma once

class MapManager : public Singleton<MapManager>
{
private:
	int PORTAL_POOL_SIZE = 4;

private:
	friend class Singleton;

	MapManager();
	~MapManager();

public:
	void Update();
	void Render();

	void Load(string file);

	float GetHeight(const Vector3& pos) const;
	void ResolveCollisions(Player* player);
	
	void OpenDoorX(int index);
	void OpenDoorZ(int index);
	void OpenAllDoorsX();
	void OpenAllDoorsZ();
	void CloseDoorX(int index);
	void CloseDoorZ(int index);
	void CloseAllDoorsX();
	void CloseAllDoorsZ();

	bool IsDoorXOpen(int index);
	bool IsDoorZOpen(int index);
	Cube* GetDoorX(int index) { return doorxs[index]; }
	Cube* GetDoorZ(int index) { return doorzs[index]; }

	void SetMonsterStageCleared(bool value) { wasMonsterStageCleared = value; }
	bool GetMonsterStageCleared() const { return wasMonsterStageCleared; }

	Vector2 GetMapSize() const { return Vector2(mapWidth, mapHeight); }
	void UpdatePortals();
	vector<Portal*>& GetPortals() { return portals; }

private:
	Cube* floor;
	vector<Cube*> walls;
	vector<Cube*> doorxs;
	vector<Cube*> doorzs;
	vector<Cube*> loweringDoors;
	vector<Portal*> portals;

	int mapWidth = 0;
	int mapHeight = 0;

	bool wasMonsterStageCleared = false;
};