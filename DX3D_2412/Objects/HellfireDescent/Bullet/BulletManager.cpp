#include "Framework.h"

BulletManager::BulletManager()
{
	playerBulletInstancing = new ModelInstancing("Bullet", SIZE);
	enemyBulletInstancing = new ModelInstancing("FireBall", SIZE);
	rocketLauncherInstancing = new ModelInstancing("Rocket", SIZE);
	unmaykrInstancing = new ModelInstancing("RedBullet", SIZE);
	bfg9000Instancing = new ModelInstancing("GreenBullet", SIZE);

	bullets.reserve(SIZE * 5);

	FOR(SIZE)
	{
		Bullet* playerBullet = new Bullet(playerBulletInstancing->Add());
		playerBullet->SetActive(false);
		playerBullet->Fire(Vector3(), Vector3(), true);
		playerBullet->SetLocalScale(0.1f, 0.1f, 0.1f);
		bullets.push_back(playerBullet);

		Bullet* enemyBullet = new Bullet(enemyBulletInstancing->Add());
		enemyBullet->SetActive(false);
		enemyBullet->Fire(Vector3(), Vector3(), false);
		enemyBullet->SetLocalScale(0.3f, 0.3f, 0.3f);
		bullets.push_back(enemyBullet);

		RocketLauncher* rocketLauncher = new RocketLauncher(rocketLauncherInstancing->Add());
		rocketLauncher->SetActive(false);
		rocketLauncher->SetLocalScale(0.4f, 0.4f, 0.4f);
		bullets.push_back(rocketLauncher);

		Unmaykr* unmaykr = new Unmaykr(unmaykrInstancing->Add());
		unmaykr->SetActive(false);
		unmaykr->SetLocalScale(0.1f, 0.1f, 0.1f);
		bullets.push_back(unmaykr);

		BFG9000* bfg9000 = new BFG9000(bfg9000Instancing->Add());
		bfg9000->SetActive(false);
		bfg9000->SetLocalScale(0.1f, 0.1f, 0.1f);
		bullets.push_back(bfg9000);
	}
}

BulletManager::~BulletManager()
{
	delete playerBulletInstancing;
	delete enemyBulletInstancing;
	delete rocketLauncherInstancing;
	delete unmaykrInstancing;
	delete bfg9000Instancing;
}

void BulletManager::Update()
{
	for (Bullet* bullet : bullets)
		bullet->Update();

	playerBulletInstancing->Update();
	enemyBulletInstancing->Update();
	rocketLauncherInstancing->Update();
	unmaykrInstancing->Update();
	bfg9000Instancing->Update();
}

void BulletManager::Render()
{
	for (Bullet* bullet : bullets)
		bullet->Render();

	playerBulletInstancing->Render();
	enemyBulletInstancing->Render();
	rocketLauncherInstancing->Render();
	unmaykrInstancing->Render();
	bfg9000Instancing->Render();
}

void BulletManager::Edit()
{
	for (Bullet* bullet : bullets)
		bullet->Edit();

	playerBulletInstancing->Edit();
	rocketLauncherInstancing->Edit();
	unmaykrInstancing->Edit();
	bfg9000Instancing->Edit();
}

void BulletManager::Fire(Vector3 pos, Vector3 direction, bool isPlayer)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive() && bullet->IsPlayerBullet() == isPlayer)
		{
			if (dynamic_cast<RocketLauncher*>(bullet) == nullptr)
			{
				bullet->Fire(pos, direction, isPlayer);
				return;
			}
		}
	}
}

void BulletManager::FireEnemyBullet(Vector3 pos, Vector3 direction)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive() && !bullet->IsPlayerBullet())
		{
			if (typeid(*bullet) == typeid(Bullet))
			{
				bullet->Fire(pos, direction, false);
				return;
			}
		}
	}
}

void BulletManager::FireRocketLauncher(Vector3 pos, Vector3 direction)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive())
		{
			RocketLauncher* rocketLauncher = dynamic_cast<RocketLauncher*>(bullet);
			if (rocketLauncher)
			{
				rocketLauncher->Fire(pos, direction, true);
				return;
			}
		}
	}
}

void BulletManager::FireUnmaykr(Vector3 pos, Vector3 direction)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive())
		{
			Unmaykr* unmaykr = dynamic_cast<Unmaykr*>(bullet);
			if (unmaykr)
			{
				unmaykr->Fire(pos, direction, true);
				return;
			}
		}
	}
}

void BulletManager::FireBFG9000(Vector3 pos, Vector3 direction, bool isPlayer, bool charged)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive())
		{
			BFG9000* bfg9000 = dynamic_cast<BFG9000*>(bullet);
			if (bfg9000)
			{
				bfg9000->Fire(pos, direction, isPlayer, charged);
				return;
			}
		}
	}
}
