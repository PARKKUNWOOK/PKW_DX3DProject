#pragma once

class Ballsita : public Weapon
{
public:
	Ballsita(Transform* transform);

	void HandleInput() override;

private:
	float fireInterval = 0.0f;
	float fireTimer = 0.0f;
};