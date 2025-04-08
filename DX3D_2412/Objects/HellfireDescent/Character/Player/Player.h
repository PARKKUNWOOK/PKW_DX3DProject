#pragma once

class Player : public SphereCollider
{
private:
	static Player* instance;

	const float GRAVITY = 9.8f;
	const float JUMP_POWER = 4.0f;
	const float INVINCIBLE_DURATION = 5.0f;

public:
	static Player* Get()
	{
		return instance;
	}

	Player();
	~Player();

	void Update();
	void Render();
	void PostRender();
	void Edit();

	void TakeDamage(int damage, Vector3 knockbackDir);
	int GetCurrentHp() { return curHp; }
	bool IsGameOver() { return isGameOver; }
	SphereCollider* GetCollider() { return this; }


private:
	void Control();
	void ChangeWeapon();
	void Fire();
	void Jump();
	void Move();
	void SetCursor();
	void CheckGameOver();

private:
	float camPitch = 0.0f;
	float moveSpeed = 5.0f;
	float rotSpeed = 1.0f;
	int maxHp = 6;
	int curHp = 6;

	bool isInvincible = false;
	float invincibleTime = 0.0f;

	bool isGameOver = false;
	bool isTouchingPortal = false;

	Vector3 velocity;
	POINT clientCenterPos;

	WeaponType currentWeaponType = WeaponType::Ballsita;
	WeaponViewModel* viewModel = nullptr;
};