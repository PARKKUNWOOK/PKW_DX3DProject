#pragma once

class CombatShotgun : public Weapon
{
public:
	CombatShotgun();
	~CombatShotgun();

	void Fire(Vector3 pos, Vector3 dir);

private:
	int pelletCount = 8;
	float spreadAngle = 10.0f;
};