#include "Framework.h"

BulletManager::BulletManager()
{
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
}

void BulletManager::Update()
{
	for (Bullet* bullet : bullets)
		bullet->Update();
}

void BulletManager::Render()
{
	for (Bullet* bullet : bullets)
		bullet->Render();
}

void BulletManager::Edit()
{
	for (Bullet* bullet : bullets)
		bullet->Edit();
}

void BulletManager::Fire(Vector3 pos, Vector3 direction, bool isPlayer)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive())
		{
			bullet->Fire(pos, direction, isPlayer);
			return;
		}
	}
}
