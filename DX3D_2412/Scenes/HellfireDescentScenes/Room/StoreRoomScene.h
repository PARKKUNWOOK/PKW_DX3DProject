#pragma once

class StoreRoomScene : public Scene
{
public:
	StoreRoomScene();
	~StoreRoomScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

	void CheckSceneTransition();

private:
	bool isTouchingPortal = false;
};