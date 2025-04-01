#pragma once

class Player;

class Bullet : public SphereCollider
{
protected:
	const float LIFE_TIME = 3.0f;

public:
	Bullet(Transform* transform);
	virtual ~Bullet();

	virtual void Update();
	virtual void Render();
	virtual void Edit();

	virtual void Fire(Vector3 pos, Vector3 dir);
	bool PlayerCollisionCheck(Player* player);

	int GetDamage() { return damage; }

protected:
	Transform* transform;
	Vector3 direction;

	float speed = 10.0f;
	Vector3 velocity;
	float lifeTime = 0.0f;
	int damage = 1;
};