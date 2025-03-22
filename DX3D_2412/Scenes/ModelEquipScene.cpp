#include "Framework.h"
#include "ModelEquipScene.h"

ModelEquipScene::ModelEquipScene()
{
	modelAnimator = new ModelAnimator("AFO");	
	modelAnimator->Load();
	modelAnimator->ReadClip("Idle");
	modelAnimator->ReadClip("Run");	
	modelAnimator->CreateTexture();

	weapon = new Model("Bow");
	weapon->Load();

	weaponSocket = new Transform();	
	weapon->SetParent(weaponSocket);

	arrow = new Model("Arrow");
	arrow->Load();

	arrowSocket = new Transform();
	arrow->SetParent(arrowSocket);
}

ModelEquipScene::~ModelEquipScene()
{
	delete modelAnimator;
	delete weapon;
	delete weaponSocket;
}

void ModelEquipScene::Update()
{
	weaponSocket->SetWorld(modelAnimator->GetTransformByNode(12));
	arrowSocket->SetWorld(modelAnimator->GetTransformByNode(36));

	modelAnimator->UpdateWorld();
	weapon->UpdateWorld();
	arrow->UpdateWorld();
}

void ModelEquipScene::PreRender()
{
}

void ModelEquipScene::Render()
{
	modelAnimator->Render();
	weapon->Render();
	arrow->Render();
}

void ModelEquipScene::PostRender()
{
}

void ModelEquipScene::GUIRender()
{
	modelAnimator->Edit();
	weapon->Edit();
	arrow->Edit();
}
