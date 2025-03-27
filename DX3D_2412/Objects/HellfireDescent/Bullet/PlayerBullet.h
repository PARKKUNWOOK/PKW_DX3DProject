#pragma once

class PlayerBullet : public Bullet
{
public:
	PlayerBullet(Transform* transform);
	~PlayerBullet() = default;

	void Update() override;

private:
	Transform* transform;
};