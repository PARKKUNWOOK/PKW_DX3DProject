#include "Framework.h"

WeaponManager::WeaponManager()
{
	ballsitaInstancing = new ModelInstancing("Bullet", SIZE);
	heavyCannonInstancing = new ModelInstancing("Bullet", SIZE);
	chainGunInstancing = new ModelInstancing("Bullet", SIZE);
	rocketInstancing = new ModelInstancing("Rocket", SIZE);
	unmaykrInstancing = new ModelInstancing("RedBullet", SIZE);
	bfg9000Instancing = new ModelInstancing("GreenBullet", SIZE);

	weapons.reserve(SIZE * 8);

	FOR(SIZE)
	{
		Ballsita* ballsita = new Ballsita(ballsitaInstancing->Add());
		ballsita->SetActive(false);
		ballsita->SetLocalScale(0.1f, 0.1f, 0.1f);
		weapons.push_back(ballsita);

		HeavyCannon* heavyCannon = new HeavyCannon(heavyCannonInstancing->Add());
		heavyCannon->SetActive(false);
		heavyCannon->SetLocalScale(0.1f, 0.1f, 0.1f);
		weapons.push_back(heavyCannon);

		ChainGun* chainGun = new ChainGun(chainGunInstancing->Add());
		chainGun->SetActive(false);
		chainGun->SetLocalScale(0.1f, 0.1f, 0.1f);
		weapons.push_back(chainGun);

		RocketLauncher* rocketLauncher = new RocketLauncher(rocketInstancing->Add());
		rocketLauncher->SetActive(false);
		rocketLauncher->SetLocalScale(0.4f, 0.4f, 0.4f);
		weapons.push_back(rocketLauncher);

		Unmaykr* unmaykr = new Unmaykr(unmaykrInstancing->Add());
		unmaykr->SetActive(false);
		unmaykr->SetLocalScale(0.1f, 0.1f, 0.1f);
		weapons.push_back(unmaykr);

		CombatShotgun* shotgun = new CombatShotgun();
		shotgun->SetActive(false);
		weapons.push_back(shotgun);

		PlasmaRifle* plasma = new PlasmaRifle();
		plasma->SetActive(false);
		weapons.push_back(plasma);

		BFG9000* bfg9000 = new BFG9000(bfg9000Instancing->Add());
		bfg9000->SetActive(false);
		bfg9000->SetLocalScale(0.1f, 0.1f, 0.1f);
		weapons.push_back(bfg9000);
	}
}

WeaponManager::~WeaponManager()
{
	delete ballsitaInstancing;
	delete heavyCannonInstancing;
	delete chainGunInstancing;
	delete rocketInstancing;
	delete unmaykrInstancing;
	delete bfg9000Instancing;
}

void WeaponManager::Update()
{
	for (Weapon* weapon : weapons)
		weapon->Update();

	ballsitaInstancing->Update();
	heavyCannonInstancing->Update();
	chainGunInstancing->Update();
	rocketInstancing->Update();
	unmaykrInstancing->Update();
	bfg9000Instancing->Update();
}

void WeaponManager::Render()
{
	for (Weapon* weapon : weapons)
		weapon->Render();

	ballsitaInstancing->Render();
	heavyCannonInstancing->Render();
	chainGunInstancing->Render();
	rocketInstancing->Render();
	unmaykrInstancing->Render();
	bfg9000Instancing->Render();
}

void WeaponManager::Edit()
{
	for (Weapon* weapon : weapons)
		weapon->Edit();

	ballsitaInstancing->Edit();
	heavyCannonInstancing->Edit();
	chainGunInstancing->Edit();
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
		BFG9000* bfg = dynamic_cast<BFG9000*>(weapon);
		if (bfg && !bfg->IsActive())
		{
			bfg->SetFireContext(pos, direction);
			bfg->Fire(pos, direction);
			return;
		}
	}
}

Weapon* WeaponManager::GetIdleWeaponByType(WeaponType type)
{
	for (Weapon* weapon : weapons)
	{
		if (weapon->GetWeaponType() == type && !weapon->IsActive())
			return weapon;
	}
	return nullptr;
}
