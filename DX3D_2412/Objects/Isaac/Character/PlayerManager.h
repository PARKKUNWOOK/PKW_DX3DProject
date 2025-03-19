#pragma once

class PlayerManager : public Singleton<PlayerManager>
{
private:
	friend class Singleton;

	PlayerManager();
	~PlayerManager();

public:
	void Update();
	void Render();
	void PostRender();
	void Edit();

	Player* GetPlayer() { return player; }

private:
	Player* player;
};