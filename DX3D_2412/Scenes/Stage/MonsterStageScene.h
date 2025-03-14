#pragma once

class MonsterStageScene : public Scene
{
public:
	MonsterStageScene();
	~MonsterStageScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void SpawnEnemies(int count);

private:
	Player* player;
	PoolingManager<AssaultEnemy>* assaultEnemyPool;
	PoolingManager<ThrowerEnemy>* throwerEnemyPool;
	const int maxEnemies = 50;
	const int spawnCount = 1;
};