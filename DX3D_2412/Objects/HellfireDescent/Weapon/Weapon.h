#pragma once

class Enemy;

enum class WeaponType
{
	Ballsita = 1,
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
	virtual void HandleInput() {}

	int GetDamage() { return damage; }
	WeaponType GetWeaponType() const { return weaponType; }

	bool CanFire() const { return fireTimer >= fireInterval; }
	void SetFireInterval(float interval) { fireInterval = interval; }
	void ResetFireTimer() { fireTimer = 0.0f; }
	virtual void SetFireContext(const Vector3& pos, const Vector3& dir)
	{
		firePosition = pos;
		fireDirection = dir;
	}

	float GetFireTimer() { return fireTimer; }
	float GetFireInterval() { return fireInterval; }

protected:
	WeaponType weaponType = WeaponType::Ballsita;
	Transform* transform;
	Vector3 direction;

	float speed = 10.0f;
	Vector3 velocity;
	float lifeTime = 0.0f;
	int damage = 1;

	Vector3 firePosition;
	Vector3 fireDirection;

	float fireInterval = 0.0f;
	float fireTimer = 0.0f;
};