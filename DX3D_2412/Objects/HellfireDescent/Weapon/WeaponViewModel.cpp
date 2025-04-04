#include "Framework.h"

WeaponViewModel::WeaponViewModel()
{
	SetParent(CAM);
}

WeaponViewModel::~WeaponViewModel()
{
	delete currentModel;
}

void WeaponViewModel::Update()
{
	UpdateWorld();

	if (currentModel)
		currentModel->UpdateWorld();
}

void WeaponViewModel::Render()
{
	if (currentModel)
		currentModel->Render();
}

void WeaponViewModel::SetModel(Model* model)
{
	if (currentModel)
		delete currentModel;

	currentModel = model;

	currentModel->SetParent(this);
	currentModel->SetLocalPosition(Vector3(0.6f, -0.3f, 0.6f));
	currentModel->SetLocalRotation(Vector3(1.6f, 11.0f, 0.0f));
	currentModel->SetLocalScale(0.02f, 0.02f, 0.02f);
}

Vector3 WeaponViewModel::GetMuzzleWorldPosition(WeaponType type)
{
	if (!currentModel)
		return GetGlobalPosition();

	Vector3 localMuzzle;

	if (type == WeaponType::Ballsita)         localMuzzle = Vector3(10.0f, -3.0f, 0.0f);
	else if (type == WeaponType::HeavyCannon) localMuzzle = Vector3(18.5f, -2.0f, -5.4f);
	else if (type == WeaponType::ChainGun)    localMuzzle = Vector3(29.0f, -2.0f, -5.4f);
	else if (type == WeaponType::RocketLauncher) localMuzzle = Vector3(2.0f, 6.0f, 0.5f);
	else if (type == WeaponType::Unmaykr)     localMuzzle = Vector3(28.0f, -1.5f, -5.0f);
	else if (type == WeaponType::BFG9000)     localMuzzle = Vector3(5.9f, 21.0f, 2.0f);
	else return currentModel->GetGlobalPosition();

	XMMATRIX world = currentModel->GetWorld();
	XMVECTOR result = XMVector3TransformCoord(localMuzzle, world);
	return Vector3(result);
}
