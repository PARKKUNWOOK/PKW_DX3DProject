#include "Framework.h"

WeaponManager::WeaponManager()
{
	pistolBulletInstancing = new ModelInstancing("Bullet", SIZE);
	rocketInstancing = new ModelInstancing("Rocket", SIZE);
	unmaykrInstancing = new ModelInstancing("RedBullet", SIZE);
	bfg9000Instancing = new ModelInstancing("GreenBullet", SIZE);

	weapons.reserve(SIZE * 4);

	FOR(SIZE)
	{
		Pistol* pistol = new Pistol(pistolBulletInstancing->Add());
		pistol->SetActive(false);
		pistol->Fire(Vector3(), Vector3());
		pistol->SetLocalScale(0.1f, 0.1f, 0.1f);
		weapons.push_back(pistol);

		RocketLauncher* rocketLauncher = new RocketLauncher(rocketInstancing->Add());
		rocketLauncher->SetActive(false);
		rocketLauncher->SetLocalScale(0.4f, 0.4f, 0.4f);
		weapons.push_back(rocketLauncher);

		Unmaykr* unmaykr = new Unmaykr(unmaykrInstancing->Add());
		unmaykr->SetActive(false);
		unmaykr->SetLocalScale(0.1f, 0.1f, 0.1f);
		weapons.push_back(unmaykr);

		BFG9000* bfg9000 = new BFG9000(bfg9000Instancing->Add());
		bfg9000->SetActive(false);
		bfg9000->SetLocalScale(0.1f, 0.1f, 0.1f);
		weapons.push_back(bfg9000);
	}
}

WeaponManager::~WeaponManager()
{
	delete pistolBulletInstancing;
	delete rocketInstancing;
	delete unmaykrInstancing;
	delete bfg9000Instancing;
}

void WeaponManager::Update()
{
	for (Weapon* weapon : weapons)
		weapon->Update();

	pistolBulletInstancing->Update();
	rocketInstancing->Update();
	unmaykrInstancing->Update();
	bfg9000Instancing->Update();
}

void WeaponManager::Render()
{
	for (Weapon* weapon : weapons)
		weapon->Render();

	pistolBulletInstancing->Render();
	rocketInstancing->Render();
	unmaykrInstancing->Render();
	bfg9000Instancing->Render();
}

void WeaponManager::Edit()
{
	for (Weapon* weapon : weapons)
		weapon->Edit();

	pistolBulletInstancing->Edit();
	rocketInstancing->Edit();
	unmaykrInstancing->Edit();
	bfg9000Instancing->Edit();
}

void WeaponManager::Fire(Vector3 pos, Vector3 direction, WeaponType type)
{
	for (Weapon* weapon : weapons)
	{
		if (!weapon->IsActive() && weapon->GetWeaponType() == type)
		{
			weapon->Fire(pos, direction);
			return;
		}
	}
}

void WeaponManager::FireBFG9000(Vector3 pos, Vector3 direction, bool charged)
{
	for (Weapon* weapon : weapons)
	{
		if (!weapon->IsActive())
		{
			BFG9000* bfg9000 = dynamic_cast<BFG9000*>(weapon);
			if (bfg9000)
			{
				bfg9000->Fire(pos, direction, charged);
				return;
			}
		}
	}
}
