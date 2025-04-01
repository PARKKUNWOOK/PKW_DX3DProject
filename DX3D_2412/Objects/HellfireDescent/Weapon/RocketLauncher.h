#pragma once

class RocketLauncher : public Weapon
{
public:
	RocketLauncher(Transform* transform);
	~RocketLauncher();

	void Update() override;
	void Render() override;
	void Fire(Vector3 pos, Vector3 dir) override;
	bool EnemyCollisionCheck(Enemy* enemy) override;

private:
	void Explode();

private:
	bool exploded = false;
	float explosionTime = 0.0f;

	Enemy* hitEnemy = nullptr;
};