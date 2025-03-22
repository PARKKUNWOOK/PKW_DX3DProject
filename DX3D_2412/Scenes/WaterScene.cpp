#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	testModel = new TestModel();
	//plane = new Plane({ 100, 100 }, 3, 3);
	//plane->GetMaterial()->SetShader(L"Landscape/Refraction.hlsl");
	//plane->SetLocalPosition(-50, 0, -50);
	//plane->UpdateWorld();
	//
	//reflection = new Reflection(plane);
	//refraction = new Refraction(L"Resources/Textures/Landscape/Wave.dds");
	water = new Water(L"Resources/Textures/Landscape/Wave.dds");
}

WaterScene::~WaterScene()
{
	delete testModel;
	//delete plane;
	//
	//delete reflection;
	//delete refraction;
	delete water;
}

void WaterScene::Update()
{
	testModel->Update();
	//plane->UpdateWorld();
	//
	////reflection->Update();
	//refraction->Update();	
	water->Update();
}

void WaterScene::PreRender()
{
	water->SetRefraction();
	testModel->Render();

	water->SetReflection();
	testModel->Render();	
}

void WaterScene::Render()
{
	testModel->Render();

	//reflection->SetRender();
	//refraction->SetRender();
	//plane->Render();
	water->Render();
}

void WaterScene::PostRender()
{
	//reflection->PostRender();
	//refraction->PostRender();	
}

void WaterScene::GUIRender()
{
	//refraction->GUIRender();
	//plane->Edit();
	water->GUIRender();
}
