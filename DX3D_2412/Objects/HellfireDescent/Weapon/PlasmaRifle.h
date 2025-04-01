#pragma once

class PlasmaRifle : public Weapon
{
public:
	PlasmaRifle();
	~PlasmaRifle();

	void Fire(Vector3 pos, Vector3 dir);

private:
	void FireChain(Vector3 origin, Vector3 direction, int chainCount, unordered_set<Enemy*>& hitEnemies);

private:
	int maxChainCount = 5;
	float chainRadius = 5.0f;
};