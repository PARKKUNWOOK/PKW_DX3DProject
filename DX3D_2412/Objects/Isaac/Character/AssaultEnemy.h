#pragma once

class AssaultEnemy : public Enemy
{
public:
	AssaultEnemy();
	~AssaultEnemy();

	void Update() override;
	void Render() override;
	void Attack(Player* player) override;

private:
	void FollowTarget() override;

private:
	bool isAssaultAttacking = false;
	int assaultAttackFrameCount = 0;
};