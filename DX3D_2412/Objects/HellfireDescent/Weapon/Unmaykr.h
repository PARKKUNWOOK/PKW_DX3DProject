#pragma once

class Unmaykr : public Weapon
{
public:
	Unmaykr(Transform* transform);
	~Unmaykr();

	void Update() override;
	void Render() override;
	void Fire(Vector3 pos, Vector3 dir) override;
	void HandleInput() override;

private:
	float fireTimer = 0.0f;
	float fireInterval = 0.2f;
};