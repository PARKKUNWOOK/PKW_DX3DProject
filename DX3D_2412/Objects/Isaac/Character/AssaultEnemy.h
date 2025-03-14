#pragma once

class AssaultEnemy : public SphereCollider
{
public:
	AssaultEnemy();
	~AssaultEnemy();

	void Update();
	void Render();
	void PostRender();
	void Edit();

	void SetTarget(Transform* target);
	void Reset();
	void TakeDamage(int damage);
	void AssaultAttack(Player* player);

	int GetCurrentHealth() { return curHp; }

private:
	void FollowTarget();
	void Death();

private:
	Transform* target;

	ModelAnimator* model;
	float speed = 3.0f;

	static int enemyCount;

	int maxHp = 20;
	int curHp = 20;

	bool isDying = false;

	bool isAssaultAttacking = false;
	int assaultAttackFrameCount = 0;

	Vector3 direction;
};