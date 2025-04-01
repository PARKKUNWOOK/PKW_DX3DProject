#pragma once

class WeaponManager : public Singleton<WeaponManager>
{
private:
	const UINT SIZE = 100;

private:
	friend class Singleton;

	WeaponManager();
	~WeaponManager();

public:
	void Update();
	void Render();
	void Edit();

	void Fire(Vector3 pos, Vector3 direction, WeaponType type);
	void FireBFG9000(Vector3 pos, Vector3 direction, bool charged);

	const vector<Weapon*>& GetWeapons() { return weapons; }

private:
	ModelInstancing* pistolBulletInstancing;
	ModelInstancing* rocketInstancing;
	ModelInstancing* unmaykrInstancing;
	ModelInstancing* bfg9000Instancing;

	vector<Weapon*> weapons;
};