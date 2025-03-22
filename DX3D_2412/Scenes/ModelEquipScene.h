#pragma once

class ModelEquipScene : public Scene
{
public:
	ModelEquipScene();
	~ModelEquipScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	ModelAnimator* modelAnimator;
	Model* weapon;
	Transform* weaponSocket;
	Model* arrow;
	Transform* arrowSocket;
};