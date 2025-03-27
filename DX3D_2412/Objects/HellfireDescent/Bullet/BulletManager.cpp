#include "Framework.h"

BulletManager::BulletManager()
{
	playerModelInstancing = new ModelInstancing("Bullet", SIZE);

	bullets.reserve(SIZE);

	FOR(SIZE)
	{
		Bullet* bullet = new Bullet();
		bullet->SetActive(false);

		bullets.push_back(bullet);
	}
}

BulletManager::~BulletManager()
{
	delete playerModelInstancing;
}

void BulletManager::Update()
{
	for (Bullet* bullet : bullets)
		bullet->Update();

	playerModelInstancing->Update();
}

void BulletManager::Render()
{
	for (Bullet* bullet : bullets)
		bullet->Render();

	playerModelInstancing->Render();
}

void BulletManager::Edit()
{
	for (Bullet* bullet : bullets)
		bullet->Edit();
	playerModelInstancing->Edit();
}

void BulletManager::Fire(Vector3 pos, Vector3 direction, bool isPlayer)
{
	for (Bullet*& bullet : bullets)
	{
		if (!bullet->IsActive())
		{
			if (isPlayer)
			{
				// 기존 bullet 해제 후 playerBullet로 대체
				delete bullet;
				bullet = new PlayerBullet(playerModelInstancing->Add());
			}

			bullet->Fire(pos, direction, isPlayer);
			return;
		}
	}
}
