#pragma once

class ShadowScene : public Scene
{
public:
	ShadowScene();
	~ShadowScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	TestModel* testModel;
	Plane* plane;

	Shadow* shadow;
};