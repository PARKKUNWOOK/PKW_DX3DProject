#pragma once

class StartStageScene : public Scene
{
public:
	StartStageScene();
	~StartStageScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	Player* player;
};