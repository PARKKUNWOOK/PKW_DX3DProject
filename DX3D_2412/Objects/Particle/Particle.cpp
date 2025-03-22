#include "Framework.h"

Particle::Particle()
{
    material = new Material();
}

Particle::~Particle()
{
    delete material;
    delete vertexBuffer;
}

void Particle::Render()
{
    if (!isActive) return;

    if (isAdditive)
        Environment::Get()->SetAdditive();
    else
        Environment::Get()->SetAlphaBlend(true);    

    Environment::Get()->SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);

    vertexBuffer->Set(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

    material->Set();
    geometryShader->Set();

    DC->Draw(particleCount, 0);

    DC->GSSetShader(nullptr, nullptr, 0);

    Environment::Get()->SetAlphaBlend(false);
    Environment::Get()->SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK_ALL);
}

void Particle::Play(Vector3 position)
{
    isActive = true;
    this->position = position;
}

void Particle::Stop()
{
    isActive = false;
}
