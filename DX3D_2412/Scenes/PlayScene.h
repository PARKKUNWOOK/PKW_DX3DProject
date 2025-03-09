#pragma once

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void SpawnEnemies();

private:
	Player* player;
	PoolingManager<Enemy>* enemyPool;
	const int maxEnemies = 30;
	const int spawnCount = 5;
};