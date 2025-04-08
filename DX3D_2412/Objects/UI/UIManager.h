#pragma once

class UIManager : public Singleton<UIManager>
{
private:
	friend class Singleton;

	const float MINING_DISTANCE = 3.0f;

private:
	UIManager();
	~UIManager();

public:
	void Update();
	void Render();	
	void Edit();

	void UpdateHpUI();

	bool IsPopup() { return isPopup; }

private:
	void CreateInGameUI();

private:
	Quad* cursor;
	Quad* hpWindowUI;
	vector<Quad*> hpUI;

	bool isPopup = false;
};