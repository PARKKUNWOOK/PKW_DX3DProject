#pragma once

class Unmaykr : public Bullet
{
public:
	Unmaykr(Transform* transform);
	~Unmaykr();

	void Update() override;
	void Render() override;
	void Fire(Vector3 pos, Vector3 dir, bool isPlayer = true) override;
};