#pragma once

class Enemy : public SphereCollider
{
public:
	Enemy();
	~Enemy();

	void Update();
	void Render();
	void PostRender();

	void SetTarget(Transform* target);
	void Reset();
	void TakeDamage(int damage);

	int GetCurrentHealth() { return curHp; }

private:
	void FollowTarget();

private:
	Transform* target;

	Cube* cube;
	float speed = 0.5f;

	static int enemyCount;
	int id;

	Quad* hpBar;
	int maxHp = 20;
	int curHp = 20;

	bool isDying = false;

	Vector3 direction;
};
