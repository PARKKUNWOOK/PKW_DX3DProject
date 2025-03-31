#pragma once

class BFG9000 : public Bullet
{
public:
	BFG9000(Transform* transform);
	~BFG9000();

	void Update() override;
	void Render() override;
	void Fire(Vector3 pos, Vector3 dir, bool isPlayer, bool charged);

	bool EnemyCollisionCheck(Enemy* enemy) override;

private:
	void Explode();
	void EmitRays();

private:
	bool isCharged = false;
	bool exploded = false;
	float explosionTime = 0.0f;
	float rayAttackTimer = 0.0f;
	float rayInterval = 0.3f;

	Enemy* hitEnemy = nullptr;
	unordered_map<Enemy*, float> lastRayHitTime;
};