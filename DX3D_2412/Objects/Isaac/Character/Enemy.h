#pragma once

class Enemy : public SphereCollider
{
public:
	Enemy();
	~Enemy();

	void Update();
	void Render();
	void PostRender();
	void Edit();

	void SetTarget(Transform* target);
	void Reset();
	void TakeDamage(int damage);

	int GetCurrentHealth() { return curHp; }

private:
	void FollowTarget();

private:
	Transform* target;

	ModelAnimator* model;
	float speed = 1.0f;

	static int enemyCount;

	int maxHp = 20;
	int curHp = 20;

	bool isDying = false;

	Vector3 direction;
};
