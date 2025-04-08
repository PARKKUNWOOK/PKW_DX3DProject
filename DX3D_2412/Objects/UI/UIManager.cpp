#include "Framework.h"

UIManager::UIManager()
{
	Font::Get()->AddStyle("Icon", L"배달의민족 을지로체 TTF", 40);
	Font::Get()->AddColor("Yellow", 0, 1, 1);

	CreateInGameUI();
}

UIManager::~UIManager()
{
	delete cursor;
	delete hpWindowUI;
}

void UIManager::Update()
{
	if (KEY->Down('I'))
	{
		isPopup = !isPopup;
		ShowCursor(isPopup);
	}

	UpdateHpUI();
}

void UIManager::Render()
{
	cursor->Render();
	hpWindowUI->Render();

	for (Quad* hp : hpUI)
		hp->Render();
}

void UIManager::Edit()
{
}

void UIManager::UpdateHpUI()
{
	int hp = Player::Get()->GetCurrentHp();

	for (int i = 0; i < hpUI.size(); i++)
	{
		if (i < hp)
			hpUI[i]->SetActive(true);
		else
			hpUI[i]->SetActive(false);
	}
}

void UIManager::CreateInGameUI()
{
	cursor = new Quad(L"Resources/Textures/UI/HellfireDescentUI/Cursor2.png");
	cursor->SetLocalPosition(CENTER);
	cursor->SetLocalScale(0.05f, 0.05f, 0.05f);
	cursor->UpdateWorld();

	hpWindowUI = new Quad(L"Resources/Textures/UI/HellfireDescentUI/CurHpUI.png");
	hpWindowUI->SetLocalPosition(SCREEN_WIDTH - 1166, SCREEN_HEIGHT - 656, 0);
	hpWindowUI->SetLocalScale(4, 4, 4);
	hpWindowUI->UpdateWorld();

	Vector2 startPos = Vector2(SCREEN_WIDTH - 1130, SCREEN_HEIGHT - 615);
	float iconSize = 32.0f;
	float spacing = 10.0f;

	for (int i = 0; i < 6; i++)
	{
		Quad* hp = new Quad(L"Resources/Textures/UI/HellfireDescentUI/HpUI.png");
		int row = i / 3;
		int col = i % 3;

		float x = startPos.x - (col * (iconSize + spacing));
		float y = startPos.y - (row * (iconSize + spacing));

		hp->SetLocalPosition(x, y, 0);
		hp->SetLocalScale(0.08f, 0.08f, 0.08f);
		hp->UpdateWorld();
		hpUI.push_back(hp);
	}
}
