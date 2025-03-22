#pragma once

class MonsterRoomScene : public Scene
{
public:
	MonsterRoomScene();
	~MonsterRoomScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

	void CheckSceneTransition();

private:
	bool isTouchingPortal = false;
};