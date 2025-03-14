#pragma once

class ThrowerEnemy : public SphereCollider
{
public:
	ThrowerEnemy();
	~ThrowerEnemy();

	void Update();
	void Render();
	void PostRender();
	void Edit();

	void SetTarget(Transform* target);
	void Reset();
	void TakeDamage(int damage);
	void ThrowAttack(Player* player);

	int GetCurrentHealth() { return curHp; }

private:
	void FollowTarget();

private:
	Transform* target;

	ModelAnimator* model;
	PoolingManager<Bullet>* bullets;

	float speed = 2.0f;
	static int enemyCount;

	int maxHp = 10;
	int curHp = 10;

	bool isDying = false;

	bool isThrowAttacking = false;
	int throwAttackFrameCount = 0;

	Vector3 direction;

	int id;
};