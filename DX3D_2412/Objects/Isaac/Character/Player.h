#pragma once

class Player : public SphereCollider
{
private:
	static Player* instance;

	const float GRAVITY = 9.8f;
	const float JUMP_POWER = 4.0f;

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

	PoolingManager<Bullet>* GetBullets();

private:
	void Control();
	void Fire();
	void Jump();
	void Move();

	void CreateBullets();

	void SetCursor();

private:
	float moveSpeed = 5.0f;
	float rotSpeed = 1.0f;

	Vector3 velocity;

	POINT clientCenterPos;

	PoolingManager<Bullet>* bullets;
};