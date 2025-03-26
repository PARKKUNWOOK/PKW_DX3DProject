#include "Framework.h"

Portal::Portal(Vector3 size)
{
    collider = new BoxCollider(size);
    collider->SetActive(false);
    
    SetActive(false);
}

Portal::~Portal()
{
    delete collider;
}

void Portal::Update()
{
    Vector3 scale = collider->GetLocalScale();
    buffer->GetData()->origin = collider->GetLocalPosition();
    buffer->GetData()->velocity = Vector3(0, 35.0f, 0);
    buffer->GetData()->distance = 100.0f;
    buffer->GetData()->turbulence = 0.1f;
    buffer->GetData()->color = Float4(1, 0, 0, 1);
    buffer->GetData()->size = scale * 1.0f;
    Snow::Update();
    collider->UpdateWorld();
}

void Portal::Render()
{
    Snow::Render();
    collider->Render();
}

void Portal::GUIRender()
{
    Snow::GUIRender();
}

void Portal::SetLocalPosition(const Vector3& pos)
{
    collider->SetLocalPosition(pos);
}

void Portal::SetLocalScale(const Vector3& scale)
{
    collider->SetLocalScale(scale);
}

Vector3 Portal::GetLocalPosition()
{
    return collider->GetLocalPosition();
}

Vector3 Portal::GetLocalScale()
{
    return collider->GetLocalScale();
}

void Portal::SetActive(bool active)
{
    collider->SetActive(active);
    Snow::SetActive(active);
}

bool Portal::IsActive()
{
    return collider->IsActive();
}
