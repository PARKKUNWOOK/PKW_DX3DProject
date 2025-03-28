#pragma once

enum class FireMode
{
	Pistol = 1,
	Heavy_Cannon,
	ChainGun
};

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
	bool IsGameOver() { return isGameOver; }
	SphereCollider* GetCollider() { return this; }

private:
	void Control();
	void Fire();
	void FireBullet();
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

	Vector3 knockbackDir;

	FireMode fireMode = FireMode::Pistol;

	float bulletInterval = 0.5f;
	float bulletTimer = 0.0f;

	float minigunStartInterval = 1.0f;
	float minigunMinInterval = 0.05f;
	float minigunDecreaseRate = 1.0f;
	float minigunElapsed = 0.0f;
};