#pragma once

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};