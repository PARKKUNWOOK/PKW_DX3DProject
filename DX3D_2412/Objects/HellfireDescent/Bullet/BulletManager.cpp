#include "Framework.h"

BulletManager::BulletManager()
{
	enemyBulletInstancing = new ModelInstancing("FireBall", SIZE);

	bullets.reserve(SIZE);

	FOR(SIZE)
	{
		Bullet* enemyBullet = new Bullet(enemyBulletInstancing->Add());
		enemyBullet->SetActive(false);
		enemyBullet->Fire(Vector3(), Vector3());
		enemyBullet->SetLocalScale(0.3f, 0.3f, 0.3f);
		bullets.push_back(enemyBullet);
	}
}

BulletManager::~BulletManager()
{
	delete enemyBulletInstancing;
}

void BulletManager::Update()
{
	for (Bullet* bullet : bullets)
		bullet->Update();

	enemyBulletInstancing->Update();
}

void BulletManager::Render()
{
	for (Bullet* bullet : bullets)
		bullet->Render();

	enemyBulletInstancing->Render();
}

void BulletManager::Edit()
{
	for (Bullet* bullet : bullets)
		bullet->Edit();

	enemyBulletInstancing->Edit();
}

void BulletManager::FireEnemyBullet(Vector3 pos, Vector3 direction)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive())
		{
			bullet->Fire(pos, direction);
			return;
		}
	}
}
