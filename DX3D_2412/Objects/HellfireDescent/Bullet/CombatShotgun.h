#pragma once

class CombatShotgun
{
public:
	CombatShotgun();
	~CombatShotgun();

	void Fire(Vector3 pos, Vector3 dir, bool isPlayer = true);

private:
	int pelletCount = 8;
	float spreadAngle = 10.0f;
	int damage = 1;
};