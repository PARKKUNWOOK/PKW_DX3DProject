#include "Framework.h"
#include "Reflection.h"

Reflection::Reflection(Transform* target)
    : target(target)
{
    renderTarget = new RenderTarget(2048, 2048);
    depthStencil = new DepthStencil(2048, 2048);

    camera = new Camera();

    quad = new Quad(Vector2(200, 200));
    quad->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
    quad->SetLocalPosition(100, 100, 0);
    Texture* texture = Texture::Add(L"Reflection", renderTarget->GetSRV());
    quad->GetMaterial()->SetDiffuseMap(texture);
    quad->UpdateWorld();
}

Reflection::~Reflection()
{
    delete renderTarget;
    delete depthStencil;
    delete camera;
    delete quad;
}

void Reflection::Update()
{
    camera->SetLocalPosition(CAM->GetLocalPosition());
    camera->SetLocalRotation(CAM->GetLocalRotation());

    Vector3 rot = camera->GetLocalRotation();
    rot.x *= -1.0f;
    camera->SetLocalRotation(rot);

    Vector3 pos = camera->GetLocalPosition();
    pos.y = target->GetLocalPosition().y * 2.0f - pos.y;
    camera->SetLocalPosition(pos);

    camera->UpdateWorld();
}

void Reflection::SetReflection()
{
    ID3D11ShaderResourceView* srv = nullptr;
    DC->PSSetShaderResources(10, 1, &srv);

    renderTarget->Set(depthStencil);
    camera->SetView();
}

void Reflection::SetRender()
{
    camera->SetView(10);
    DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
}

void Reflection::PostRender()
{
    quad->Render();
}
