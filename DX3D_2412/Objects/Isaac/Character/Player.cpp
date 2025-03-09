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
    SetCursor();
    Control();
    Fire();
    Jump();
    Move();

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

        bullet->Fire(firePosition, fireDirection);
    }
}

void Player::Jump()
{
    velocity.y -= GRAVITY * DELTA;

    float bottomHeight = BlockManager::Get()->GetHeight(localPosition);

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
