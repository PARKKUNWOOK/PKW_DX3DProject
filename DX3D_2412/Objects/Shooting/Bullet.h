#pragma once

class Enemy;
class Player;

class Bullet : public SphereCollider
{
private:
	const float LIFE_TIME = 3.0f;

public:
	Bullet();
	~Bullet();

	void Update();
	void Render();

	void Fire(Vector3 pos, Vector3 dir, bool isPlayer);
	bool EnemyCollisionCheck(Enemy* enemy);
	bool PlayerCollisionCheck(Player* player);

	bool IsPlayerBullet() { return isPlayerBullet; }

private:
	Sphere* sphere;
	Vector3 direction;

	float speed = 10.0f;
	Vector3 velocity;
	float lifeTime = 0.0f;
	int damage = 0;

	bool isPlayerBullet = false;
};