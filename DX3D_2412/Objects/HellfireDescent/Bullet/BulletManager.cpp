#include "Framework.h"

BulletManager::BulletManager()
{
	playerBulletInstancing = new ModelInstancing("Bullet", SIZE);
	enemyBulletInstancing = new ModelInstancing("FireBall", SIZE);

	bullets.reserve(SIZE * 2);

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
	}
}

BulletManager::~BulletManager()
{
	delete playerBulletInstancing;
}

void BulletManager::Update()
{
	for (Bullet* bullet : bullets)
		bullet->Update();

	playerBulletInstancing->Update();
	enemyBulletInstancing->Update();
}

void BulletManager::Render()
{
	for (Bullet* bullet : bullets)
		bullet->Render();

	playerBulletInstancing->Render();
	enemyBulletInstancing->Render();
}

void BulletManager::Edit()
{
	playerBulletInstancing->Edit();

	for (Bullet* bullet : bullets)
		bullet->Edit();
}

void BulletManager::Fire(Vector3 pos, Vector3 direction, bool isPlayer)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive() && bullet->IsPlayerBullet() == isPlayer)
		{
			bullet->Fire(pos, direction, isPlayer);
			return;
		}
	}
}
