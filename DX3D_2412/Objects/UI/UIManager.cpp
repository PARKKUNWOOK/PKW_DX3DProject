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
	delete hotBar;
}

void UIManager::Update()
{
	if (KEY->Down('I'))
	{
		isPopup = !isPopup;
		ShowCursor(isPopup);
	}

	SelectBuildBlock();
	SelectBlock();
}

void UIManager::Render()
{
	cursor->Render();
	hotBar->Render();
	hotBarSelect->Render();	
}

void UIManager::Edit()
{
}

void UIManager::Mining()
{
}

void UIManager::Build()
{
}

void UIManager::RefreshQuickSlotIcons()
{
}

void UIManager::SelectBlock()
{
}

void UIManager::SelectBuildBlock()
{
	FOR(9)
	{
		int key = '1';

		if (KEY->Down(key + i))
		{
			selectBuildBlockIndex = i;
			hotBarSelect->UpdateWorld();
		}
	}
}

void UIManager::OpenInventory()
{
}

void UIManager::CreateInGameUI()
{
	cursor = new Quad(L"Resources/Textures/UI/cursor.png");
	cursor->SetLocalPosition(CENTER);
	cursor->UpdateWorld();

	hotBar = new Quad(L"Resources/Textures/UI/HotBar.png");
	hotBar->SetTag("HotBar");
	hotBar->Load();

	hotBarSelect = new Quad(L"Resources/Textures/UI/HotBarSelect.png");
	hotBarSelect->SetLocalScale(3, 3, 3);
	hotBarSelect->UpdateWorld();
}
