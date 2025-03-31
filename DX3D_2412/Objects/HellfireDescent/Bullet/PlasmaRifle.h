#pragma once

class PlasmaRifle
{
public:
	PlasmaRifle();
	~PlasmaRifle();

	void Fire(Vector3 pos, Vector3 dir, bool isPlayer = true);

private:
	void FireChain(Vector3 origin, Vector3 direction, bool isPlayer, int chainCount, unordered_set<Enemy*>& hitEnemies);

private:
	int damage = 5;
	int maxChainCount = 5;
	float chainRadius = 5.0f;
};