#pragma once

class BulletManager : public Singleton<BulletManager>
{
private:
	const UINT SIZE = 100;

private:
	friend class Singleton;

	BulletManager();
	~BulletManager();

public:
	void Update();
	void Render();
	void Edit();

	void Fire(Vector3 pos, Vector3 direction, bool isPlayer);
	void FireEnemyBullet(Vector3 pos, Vector3 direction);
	void FireRocketLauncher(Vector3 pos, Vector3 direction);
	void FireUnmaykr(Vector3 pos, Vector3 direction);
	void FireBFG9000(Vector3 pos, Vector3 direction, bool isPlayer, bool charged);

	const vector<Bullet*>& GetBullets() { return bullets; }

private:
	ModelInstancing* playerBulletInstancing;
	ModelInstancing* enemyBulletInstancing;
	ModelInstancing* rocketLauncherInstancing;
	ModelInstancing* unmaykrInstancing;
	ModelInstancing* bfg9000Instancing;

	vector<Bullet*> bullets;
};