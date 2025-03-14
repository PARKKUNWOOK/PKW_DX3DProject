#pragma once

class MapManager : public Singleton<MapManager>
{
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

private:
	Cube* floor;
	vector<Cube*> walls;
	vector<Cube*> doorxs;
	vector<Cube*> doorzs;
	vector<Cube*> loweringDoors;
};