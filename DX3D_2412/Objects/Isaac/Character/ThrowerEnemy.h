#pragma once

class ThrowerEnemy : public Enemy
{
public:
	ThrowerEnemy();
	~ThrowerEnemy();

	void Update() override;
	void Render() override;
	void Attack(Player* player) override;

private:
	void FollowTarget() override;

private:
	PoolingManager<Bullet>* bullets;
	bool isThrowAttacking = false;
	int throwAttackFrameCount = 0;
};