#include "Framework.h"
#include "ShadowScene.h"

ShadowScene::ShadowScene()
{
	testModel = new TestModel();
	plane = new Plane({ 100, 100 }, 3, 3);	
	plane->SetLocalPosition(-50, 0, -50);
	plane->UpdateWorld();
	plane->GetMaterial()->SetShader(L"Light/Shadow.hlsl");

	shadow = new Shadow();
}

ShadowScene::~ShadowScene()
{
	delete testModel;
	delete plane;
	delete shadow;
}

void ShadowScene::Update()
{
	testModel->Update();
}

void ShadowScene::PreRender()
{
	shadow->SetRenderTarget();
	testModel->SetPixelShader(L"Light/DepthMap.hlsl");
	testModel->Render();
}

void ShadowScene::Render()
{
	testModel->SetPixelShader(L"Light/Light.hlsl");
	testModel->Render();

	shadow->SetRender();
	plane->Render();
}

void ShadowScene::PostRender()
{
	shadow->PostRender();
}

void ShadowScene::GUIRender()
{
}
