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
	void MakeDoorsTransparent();

private:
	Cube* floor;
	vector<Cube*> walls;
	vector<Cube*> doors;
	vector<Cube*> loweringDoors;
};