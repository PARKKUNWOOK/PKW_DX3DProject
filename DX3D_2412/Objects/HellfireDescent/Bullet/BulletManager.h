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

	const vector<Bullet*>& GetBullets() { return bullets; }

private:
	ModelInstancing* playerModelInstancing;
	vector<Bullet*> bullets;
};