#pragma once

class WeaponViewModel : public Transform
{
public:
	WeaponViewModel();
	~WeaponViewModel();

	void Update();
	void Render();

	void SetModel(Model* model);

	Vector3 GetMuzzleWorldPosition(WeaponType type);
	


private:
	Model* currentModel = nullptr;
};