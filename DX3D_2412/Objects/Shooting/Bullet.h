#pragma once

class Enemy;

class Bullet : public SphereCollider
{
private:
	const float LIFE_TIME = 3.0f;

public:
	Bullet();
	~Bullet();

	void Update();
	void Render();

	void Fire(Vector3 pos, Vector3 dir);
	bool CollisionCheck(Enemy* enemy);

private:
	Sphere* sphere;

	float speed = 10.0f;
	Vector3 velocity;
	float lifeTime = 0.0f;
	int damage = 3;
};