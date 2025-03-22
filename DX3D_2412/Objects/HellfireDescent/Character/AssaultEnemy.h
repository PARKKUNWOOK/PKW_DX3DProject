#pragma once

class AssaultEnemy : public Enemy
{
public:
	AssaultEnemy();
	~AssaultEnemy();

	void Render() override;
	void Attack(Player* player) override;

	void AttackAction() override;

private:
	void FollowTarget() override;

private:
	bool isAssaultAttacking = false;
	int assaultAttackFrameCount = 0;
};