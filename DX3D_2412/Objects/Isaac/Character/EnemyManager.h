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
	void SpawnEnemies(int count, Player* player);
	bool IsAllDead();

private:
	Vector3 GetRandomSpawnPosition();

private:
	list<AssaultEnemy*> assaultEnemies;
	list<ThrowerEnemy*> throwerEnemies;
	int maxEnemies = 10;
};