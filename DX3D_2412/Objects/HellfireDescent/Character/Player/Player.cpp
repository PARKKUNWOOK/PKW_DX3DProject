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
}

Player::~Player()
{
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
    bulletTimer += DELTA;

    if (KEY->Down('1'))
    {
        fireMode = FireMode::Pistol;
    }
    if (KEY->Down('2'))
    {
        fireMode = FireMode::Heavy_Cannon;
        bulletInterval = 0.1f;
    }
    if (KEY->Down('3'))
    {
        fireMode = FireMode::ChainGun;
        bulletInterval = minigunStartInterval;
    }
}

void Player::Render()
{
    Collider::Render();
    BulletManager::Get()->Render();
}

void Player::PostRender()
{
}

void Player::Edit()
{
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

void Player::Fire()
{
    bool isFiring = KEY->Press(VK_LBUTTON);
    bool isClicked = KEY->Down(VK_LBUTTON);

    switch (fireMode)
    {
    case FireMode::Pistol:
        if (isClicked && bulletTimer >= bulletInterval)
        {
            FireBullet();
            bulletTimer = 0.0f;
        }
        break;

    case FireMode::Heavy_Cannon:
        if (isFiring && bulletTimer >= bulletInterval)
        {
            FireBullet();
            bulletTimer = 0.0f;
        }
        break;

    case FireMode::ChainGun:
        if (isFiring)
        {
            minigunElapsed += DELTA;

            bulletInterval = minigunStartInterval * exp(-1.0f * minigunElapsed);

            if (bulletInterval < minigunMinInterval)
                bulletInterval = minigunMinInterval;

            if (bulletTimer >= bulletInterval)
            {
                FireBullet();
                bulletTimer = 0.0f;
            }
        }
        else
        {
            bulletInterval = minigunStartInterval;
            minigunElapsed = 0.0f;
        }
        break;
    }

    //if (KEY->Down(VK_LBUTTON) && bulletTimer >= bulletInterval)
    //{
    //    Vector3 firePosition = GetLocalPosition();
    //    Vector3 fireDirection = CAM->GetForward();
    //    fireDirection.Normalize();
    //
    //    BulletManager::Get()->Fire(firePosition, fireDirection, true);
    //
    //    bulletTimer = 0.0f;
    //}
}

void Player::FireBullet()
{
    Vector3 firePosition = GetLocalPosition();
    Vector3 fireDirection = CAM->GetForward();
    fireDirection.Normalize();

    BulletManager::Get()->Fire(firePosition, fireDirection, true);
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

    SetLocalPosition(newPosition);

    MapManager::Get()->ResolveCollisions(this);
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
