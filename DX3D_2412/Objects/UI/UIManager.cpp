#include "Framework.h"

UIManager::UIManager()
{
	Font::Get()->AddStyle("Icon", L"����ǹ��� ������ü TTF", 40);
	Font::Get()->AddColor("Yellow", 0, 1, 1);

	CreateInGameUI();
}

UIManager::~UIManager()
{
	delete cursor;
}

void UIManager::Update()
{
	if (KEY->Down('I'))
	{
		isPopup = !isPopup;
		ShowCursor(isPopup);
	}
}

void UIManager::Render()
{
	cursor->Render();
}

void UIManager::Edit()
{
}

void UIManager::CreateInGameUI()
{
	cursor = new Quad(L"Resources/Textures/UI/cursor.png");
	cursor->SetLocalPosition(CENTER);
	cursor->UpdateWorld();
}
