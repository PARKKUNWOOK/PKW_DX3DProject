#include "Framework.h"

Player* Player::instance = nullptr;

Player::Player()
{
    instance = this;

    tag = "Player";

    localPosition.y = radius;

    clientCenterPos = { SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1 };
    ClientToScreen(hWnd, &clientCenterPos);
    SetCursorPos(clientCenterPos.x, clientCenterPos.y);
    ShowCursor(false);

    CAM->SetLocalPosition(0, 0, 0);
    CAM->SetLocalRotation(0, 0, 0);

    viewModel = new WeaponViewModel();
    Model* pistolModel = new Model("Ballsita");
    viewModel->SetModel(pistolModel);
}

Player::~Player()
{
    delete viewModel;
}

void Player::Update()
{
    if (isGameOver) return;

    if (UIManager::Get()->IsPopup())
        return;

    SetCursor();
    Control();
    ChangeWeapon();
    Fire();
    Jump();
    Move();

    if (isInvincible)
    {
        invincibleTime -= DELTA;
        if (invincibleTime <= 0)
        {
            isInvincible = false;
        }
    }

    UpdateWorld();

    Vector3 eyeOffset = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 camPos = GetGlobalPosition() + eyeOffset;
    CAM->SetLocalPosition(camPos);

    Vector3 playerRot = GetLocalRotation();
    CAM->SetLocalRotation(Vector3(camPitch, playerRot.y, 0.0f));

    BulletManager::Get()->Update();
    WeaponManager::Get()->Update();
    viewModel->Update();
}

void Player::Render()
{
    Collider::Render();
    BulletManager::Get()->Render();
    WeaponManager::Get()->Render();
    viewModel->Render();
}

void Player::PostRender()
{
}

void Player::Edit()
{
    ImGui::Text("HP: %d / %d", curHp, maxHp);

    const char* weaponName = "";
    switch (currentWeaponType)
    {
    case WeaponType::Ballsita:          weaponName = "Ballsita"; break;
    case WeaponType::HeavyCannon:     weaponName = "Heavy Cannon"; break;
    case WeaponType::ChainGun:        weaponName = "Chaingun"; break;
    case WeaponType::RocketLauncher:  weaponName = "Rocket Launcher"; break;
    case WeaponType::Unmaykr:         weaponName = "Unmaykr"; break;
    case WeaponType::CombatShotgun:   weaponName = "Combat Shotgun"; break;
    case WeaponType::PlasmaRifle:     weaponName = "Plasma Rifle"; break;
    case WeaponType::BFG9000:         weaponName = "BFG9000"; break;
    }

    ImGui::Text("Current Weapon: %s", weaponName);

    if (currentWeaponType == WeaponType::BFG9000)
    {
        Weapon* weapon = WeaponManager::Get()->GetIdleWeaponByType(WeaponType::BFG9000);

        if (weapon != nullptr)
        {
            BFG9000* bfg = dynamic_cast<BFG9000*>(weapon);
            if (bfg != nullptr)
            {
                bool charging = bfg->IsCharging();
                float timer = bfg->GetChargeTime();

                ImGui::Text("BFG Charging: %s", charging ? "True" : "False");
                ImGui::Text("Charge Time: %.2f sec", timer);
            }
        }
    }
}

void Player::TakeDamage(int damage, Vector3 knockbackDir)
{
    if (isInvincible || isGameOver) return;

    curHp -= damage;
    if (curHp < 0) curHp = 0;

    isInvincible = true;
    invincibleTime = INVINCIBLE_DURATION;

    knockbackDir.Normalize();
    SetLocalPosition(GetLocalPosition() + knockbackDir * 2.0f);

    CheckGameOver();
}

void Player::Control()
{
    Vector3 dir;

    if (KEY->Press('W'))
        dir += GetForward();
    if (KEY->Press('S'))
        dir += GetBack();
    if (KEY->Press('A'))
        dir += GetLeft();
    if (KEY->Press('D'))
        dir += GetRight();

    dir.Normalize();

    velocity.x = dir.x;
    velocity.z = dir.z;

    Vector3 delta = mousePos - CENTER;

    Rotate(Vector3::Up(), delta.x * rotSpeed * DELTA);
    camPitch -= delta.y * rotSpeed * DELTA;

    if (KEY->Down(VK_SPACE))
    {
        velocity.y = JUMP_POWER;
    }
}

void Player::ChangeWeapon()
{
    WeaponType prevType = currentWeaponType;

    if (KEY->Down('1')) currentWeaponType = WeaponType::Ballsita;
    if (KEY->Down('2')) currentWeaponType = WeaponType::HeavyCannon;
    if (KEY->Down('3')) currentWeaponType = WeaponType::ChainGun;
    if (KEY->Down('4')) currentWeaponType = WeaponType::RocketLauncher;
    if (KEY->Down('5')) currentWeaponType = WeaponType::Unmaykr;
    if (KEY->Down('6')) currentWeaponType = WeaponType::CombatShotgun;
    if (KEY->Down('7')) currentWeaponType = WeaponType::PlasmaRifle;
    if (KEY->Down('8')) currentWeaponType = WeaponType::BFG9000;

    if (prevType != currentWeaponType)
    {
        Model* newModel = nullptr;
        switch (currentWeaponType)
        {
        case WeaponType::Ballsita:          newModel = new Model("Ballsita"); break;
        case WeaponType::HeavyCannon:     newModel = new Model("Heavy_Cannon"); break;
        case WeaponType::ChainGun:        newModel = new Model("ChainGun"); break;
        case WeaponType::RocketLauncher:  newModel = new Model("Rocket_Launcher"); break;
        case WeaponType::Unmaykr:         newModel = new Model("Unmaykr"); break;
        case WeaponType::CombatShotgun:   newModel = new Model("Combat_Shotgun"); break;
        case WeaponType::PlasmaRifle:     newModel = new Model("Plasma_Rifle"); break;
        case WeaponType::BFG9000:         newModel = new Model("BFG9000"); break;
        }

        if (newModel != nullptr)
            viewModel->SetModel(newModel);
    }
}

void Player::Fire()
{
    Weapon* weapon = WeaponManager::Get()->GetIdleWeaponByType(currentWeaponType);
    if (weapon == nullptr) return;

    Ray ray(CAM->GetGlobalPosition(), CAM->GetForward());
    RaycastHit hit;
    bool hasHit = false;
    float minDistance = FLT_MAX;

    //1. Enemy 충돌 검사
    for (Enemy* enemy : EnemyManager::Get()->GetAllEnemies())
    {
        if (!enemy->IsActive()) continue;

        RaycastHit tempHit;
        if (enemy->IsRayCollision(ray, &tempHit))
        {
            float distance = (tempHit.point - ray.origin).Magnitude();
            if (distance < minDistance)
            {
                minDistance = distance;
                hit = tempHit;
                hasHit = true;
            }
        }
    }

    //2. Wall/문 충돌 검사
    if (!hasHit)
    {
        for (Cube* wall : MapManager::Get()->GetWalls())
        {
            RaycastHit tempHit;
            if (wall->GetCollider()->IsRayCollision(ray, &tempHit))
            {
                float distance = (tempHit.point - ray.origin).Magnitude();
                if (distance < minDistance)
                {
                    minDistance = distance;
                    hit = tempHit;
                    hasHit = true;
                }
            }
        }

        for (Cube* door : MapManager::Get()->GetDoors())
        {
            RaycastHit tempHit;
            if (door->GetCollider()->IsRayCollision(ray, &tempHit))
            {
                float distance = (tempHit.point - ray.origin).Magnitude();
                if (distance < minDistance)
                {
                    minDistance = distance;
                    hit = tempHit;
                    hasHit = true;
                }
            }
        }
    }

    if (hasHit)
    {
        Vector3 muzzlePos = viewModel->GetMuzzleWorldPosition(currentWeaponType);
        Vector3 direction = (hit.point - muzzlePos).GetNormalized();

        weapon->SetFireContext(muzzlePos, direction);
        weapon->HandleInput();
    }
}

void Player::Jump()
{
    velocity.y -= GRAVITY * DELTA;

    float bottomHeight = MapManager::Get()->GetHeight(localPosition);

    if (velocity.y < 0 && localPosition.y - radius <= bottomHeight)
    {
        localPosition.y = bottomHeight + radius;

        if (velocity.y < 0)
            velocity.y = 0.0f;
    }
}

void Player::Move()
{
    Vector3 newPosition = localPosition + (velocity * moveSpeed * DELTA);

    Vector3 horizontalPosition = newPosition;
    horizontalPosition.y = localPosition.y;

    newPosition.x = horizontalPosition.x;
    newPosition.z = horizontalPosition.z;

    float totalOverlab = 0.0f;
    Vector3 dir = MapManager::Get()->ResolveCollisions(this, totalOverlab);

    SetLocalPosition(newPosition + dir * totalOverlab);
}

void Player::SetCursor()
{
    SetCursorPos(clientCenterPos.x, clientCenterPos.y);
}

void Player::CheckGameOver()
{
    if (curHp <= 0)
    {
        isGameOver = true;

        int result = MessageBoxA(nullptr, "Game Over!", "Game Over", MB_OK);
        if (result == IDOK)
        {
            PostQuitMessage(0);
        }
    }
}
