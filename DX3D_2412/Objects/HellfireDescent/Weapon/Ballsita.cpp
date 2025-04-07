#include "Framework.h"

Ballsita::Ballsita(Transform* transform)
{
	weaponType = WeaponType::Ballsita;

	SetTag(transform->GetTag() + "_Collider");
	transform->SetParent(this);
	transform->SetTag("Ballsita_0");
	transform->Load();

	damage = 1;
	fireInterval = 1.0f;
}

void Ballsita::HandleInput()
{
	fireTimer += DELTA;

	if (!KEY->Down(VK_LBUTTON)) return;
	if (fireTimer < fireInterval) return;

	WeaponManager::Get()->Fire(firePosition, fireDirection, weaponType);
	fireTimer = 0.0f;
}
