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

    CreateBullets();

    CAM->SetLocalPosition(0, 0, 0);
    CAM->SetLocalRotation(0, 0, 0);
    CAM->SetParent(this);
}

Player::~Player()
{
    delete bullets;
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
    bullets->Update();
}

void Player::Render()
{
    Collider::Render();
    bullets->Render();
}

void Player::PostRender()
{
}

void Player::Edit()
{
}

PoolingManager<Bullet>* Player::GetBullets()
{
    return bullets;
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
    CAM->Rotate(Vector3::Left(), delta.y * rotSpeed * DELTA);

    if (KEY->Down(VK_SPACE))
    {
        velocity.y = JUMP_POWER;
    }
}

void Player::Fire()
{
    if (KEY->Down(VK_LBUTTON))
    {
        Bullet* bullet = bullets->Pop();
        if (!bullet) return;

        Vector3 firePosition = GetLocalPosition();

        Vector3 fireDirection = CAM->GetForward();
        fireDirection.Normalize();

        bullet->Fire(firePosition, fireDirection, true);
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

    SetLocalPosition(newPosition);

    MapManager::Get()->ResolveCollisions(this);
}

void Player::CreateBullets()
{
    bullets = new PoolingManager<Bullet>();
    bullets->Create(10);
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
