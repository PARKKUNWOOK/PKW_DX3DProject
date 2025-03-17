#pragma once

class EnemyManager : public Singleton<EnemyManager>
{
private:
	friend class Singleton;

	EnemyManager();
	~EnemyManager();

public:
	void Update();
	void Render();
	void Edit();
	void SpawnEnemies(int count, Player* player);
	bool IsAllDead();

private:
	Vector3 GetRandomSpawnPosition();

private:
	list<AssaultEnemy*> assaultEnemies;
	list<ThrowerEnemy*> throwerEnemies;

	//map<int, vector<Enemy*>> enemies;

	int maxEnemies = 10;
};