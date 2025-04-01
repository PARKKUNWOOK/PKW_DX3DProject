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

    combatShotgun = new CombatShotgun();
    plasmaRifle = new PlasmaRifle();
}

Player::~Player()
{
    delete combatShotgun;
    delete plasmaRifle;
}

void Player::Update()
{
    if (isGameOver) return;

    if (UIManager::Get()->IsPopup())
        return;

    SetCursor();
    Control();
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
    bulletTimer += DELTA;
    
    ChangeWeapon();
}

void Player::Render()
{
    Collider::Render();
    BulletManager::Get()->Render();
    WeaponManager::Get()->Render();
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
    case WeaponType::Pistol:          weaponName = "Pistol"; break;
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
        ImGui::Text("[BFG9000]");
        ImGui::Text("Charging: %s", bfgCharging ? "Yes" : "No");
        ImGui::Text("Charge Time: %.2f", bfgChargeTime);
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
    if (KEY->Down('1'))
    {
        currentWeaponType = WeaponType::Pistol;
        bulletInterval = 0.5f;
    }
    if (KEY->Down('2'))
    {
        currentWeaponType = WeaponType::HeavyCannon;
        bulletInterval = 0.1f;
    }
    if (KEY->Down('3'))
    {
        currentWeaponType = WeaponType::ChainGun;
        bulletInterval = minigunStartInterval;
    }
    if (KEY->Down('4'))
    {
        currentWeaponType = WeaponType::RocketLauncher;
    }
    if (KEY->Down('5'))
    {
        currentWeaponType = WeaponType::Unmaykr;
        bulletInterval = 0.1f;
    }
    if (KEY->Down('6'))
    {
        currentWeaponType = WeaponType::CombatShotgun;
        bulletInterval = 2.0f;
    }
    if (KEY->Down('7'))
    {
        currentWeaponType = WeaponType::PlasmaRifle;
        bulletInterval = 1.0f;
    }
    if (KEY->Down('8'))
    {
        currentWeaponType = WeaponType::BFG9000;
        bulletInterval = 0.1f;
    }
}

void Player::Fire()
{
    bool isFiring = KEY->Press(VK_LBUTTON);
    bool isClicked = KEY->Down(VK_LBUTTON);

    Vector3 firePosition = GetLocalPosition();
    Vector3 fireDirection = CAM->GetForward();
    fireDirection.Normalize();

    switch (currentWeaponType)
    {
    case WeaponType::Pistol:
        if (isClicked && bulletTimer >= bulletInterval)
        {
            WeaponManager::Get()->Fire(firePosition, fireDirection, WeaponType::Pistol);
            bulletTimer = 0.0f;
        }
        break;
    case WeaponType::HeavyCannon:
        if (isFiring && bulletTimer >= bulletInterval)
        {
            WeaponManager::Get()->Fire(firePosition, fireDirection, WeaponType::Pistol);
            bulletTimer = 0.0f;
        }
        break;
    case WeaponType::ChainGun:
        if (isFiring)
        {
            minigunElapsed += DELTA;

            bulletInterval = minigunStartInterval * exp(-1.0f * minigunElapsed);

            if (bulletInterval < minigunMinInterval)
                bulletInterval = minigunMinInterval;

            if (bulletTimer >= bulletInterval)
            {
                WeaponManager::Get()->Fire(firePosition, fireDirection, WeaponType::Pistol);
                bulletTimer = 0.0f;
            }
        }
        else
        {
            bulletInterval = minigunStartInterval;
            minigunElapsed = 0.0f;
        }
        break;
    case WeaponType::RocketLauncher:
        if (isClicked && bulletTimer >= rocketInterval)
        {
            WeaponManager::Get()->Fire(firePosition, fireDirection, WeaponType::RocketLauncher);
            bulletTimer = 0.0f;
        }
        break;
    case WeaponType::Unmaykr:
        if (isFiring && bulletTimer >= bulletInterval)
        {
            WeaponManager::Get()->Fire(firePosition, fireDirection, WeaponType::Unmaykr);
            bulletTimer = 0.0f;
        }
        break;
    case WeaponType::CombatShotgun:
        if (isClicked && bulletTimer >= bulletInterval)
        {
            if (combatShotgun != nullptr)
                combatShotgun->Fire(firePosition, fireDirection);
            bulletTimer = 0.0f;
        }
        break;
    case WeaponType::PlasmaRifle:
        if (isClicked && bulletTimer >= bulletInterval)
        {
            if (plasmaRifle != nullptr)
                plasmaRifle->Fire(firePosition, fireDirection);
            bulletTimer = 0.0f;
        }
        break;
    case WeaponType::BFG9000:
    {
        static bool isCharging = false;
        static float chargeTime = 0.0f;

        if (KEY->Down(VK_LBUTTON))
        {
            isCharging = true;
            chargeTime = 0.0f;
        }

        if (KEY->Press(VK_LBUTTON) && isCharging)
        {
            chargeTime += DELTA;
        }

        if (KEY->Up(VK_LBUTTON) && isCharging)
        {
            isCharging = false;
            
            if (bulletTimer >= (chargeTime >= 3.0f ? 3.0f : 0.5f))
            {
                bool chargedShot = chargeTime >= 3.0f;
            
                WeaponManager::Get()->FireBFG9000(firePosition, fireDirection, chargedShot);
            
                bulletTimer = 0.0f;
            }
            
            chargeTime = 0.0f;
        }
        this->bfgCharging = isCharging;
        this->bfgChargeTime = chargeTime;
    }
    break;
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
