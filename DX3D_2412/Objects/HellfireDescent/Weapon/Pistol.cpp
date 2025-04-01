#include "Framework.h"

Pistol::Pistol(Transform* transform)
{
	weaponType = WeaponType::Pistol;

	SetTag(transform->GetTag() + "_Collider");
	transform->SetParent(this);
	transform->SetTag("Pistol_0");
	transform->Load();
}
