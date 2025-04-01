#pragma once

class Enemy;

enum class WeaponType
{
	Pistol = 1,
	HeavyCannon,
	ChainGun,
	RocketLauncher,
	Unmaykr,
	CombatShotgun,
	PlasmaRifle,
	BFG9000
};

class Weapon : public SphereCollider
{
protected:
	const float LIFE_TIME = 3.0f;

public:
	Weapon();
	virtual ~Weapon();

	virtual void Update();
	virtual void Render();
	virtual void Edit();

	virtual void Fire(Vector3 pos, Vector3 dir);
	virtual bool EnemyCollisionCheck(Enemy* enemy);

	int GetDamage() { return damage; }
	WeaponType GetWeaponType() const { return weaponType; }

protected:
	WeaponType weaponType = WeaponType::Pistol;
	Transform* transform;
	Vector3 direction;

	float speed = 10.0f;
	Vector3 velocity;
	float lifeTime = 0.0f;
	int damage = 1;
};