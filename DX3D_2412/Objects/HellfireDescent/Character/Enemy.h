#pragma once

class Enemy : public SphereCollider
{
public:
	Enemy();
	virtual ~Enemy();

	void Update();
	virtual void Render();
	virtual void PostRender();
	virtual void Edit();

	virtual void AttackAction() = 0;

	void SetTarget(Transform* target);
	void TakeDamage(int damage);
	virtual void Attack(Player* player) = 0;

	int GetCurrentHealth() { return curHp; }

protected:
	virtual void FollowTarget() = 0;
	void Death();

protected:
	Transform* target;
	ModelAnimator* model;

	float speed;
	static int enemyCount;
	int maxHp;
	int curHp;

	bool isDying = false;
	Vector3 direction;
};