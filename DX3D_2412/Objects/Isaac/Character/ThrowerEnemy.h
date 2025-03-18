#pragma once

class ThrowerEnemy : public Enemy
{
public:
	ThrowerEnemy();
	~ThrowerEnemy();

	void Render() override;
	void Attack(Player* player) override;

	void AttackAction() override;

private:
	void FollowTarget() override;

private:
	bool isThrowAttacking = false;
	int throwAttackFrameCount = 0;
};