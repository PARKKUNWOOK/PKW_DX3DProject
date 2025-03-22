#include "Framework.h"

Snow::Snow()
{
    material->SetShader(L"Geometry/Snow.hlsl");
    material->SetDiffuseMap(L"Resources/Textures/FX/Snow.png");

    geometryShader = Shader::AddGS(L"Geometry/Snow.hlsl");

    buffer = new WeatherBuffer();

    Create();
}

Snow::~Snow()
{
    delete buffer;
}

void Snow::Update()
{
    buffer->GetData()->time += DELTA;
}

void Snow::Render()
{
    buffer->SetVS(10);
    Particle::Render();
}

void Snow::GUIRender()
{
    ImGui::Text("Snow Option");
    ImGui::DragFloat3("Velocity", (float*)&buffer->GetData()->velocity, 0.1f);
    ImGui::SliderFloat("Distance", (float*)&buffer->GetData()->distance, 0.0f, 500.0f);
    ImGui::SliderFloat("Turbulence", (float*)&buffer->GetData()->turbulence, 0.0f, 10.0f);
    ImGui::DragFloat3("Origin", (float*)&buffer->GetData()->origin, 0.1f);
    ImGui::DragFloat3("Size", (float*)&buffer->GetData()->size, 0.1f);
    ImGui::ColorEdit3("Color", (float*)&buffer->GetData()->color);
}

void Snow::Create()
{
    particleCount = SIZE;

    vertices.resize(particleCount);

    FOR(particleCount)
    {
        Float2 size;
        size.x = GameMath::Random(0.5f, 2.0f);
        size.y = size.x;

        Float3 pos;
        pos.x = GameMath::Random(-buffer->GetData()->size.x, buffer->GetData()->size.x);
        pos.y = GameMath::Random(-buffer->GetData()->size.y, buffer->GetData()->size.y);
        pos.z = GameMath::Random(-buffer->GetData()->size.z, buffer->GetData()->size.z);

        vertices[i].pos = pos;
        vertices[i].size = size;

        vertices[i].random.x = GameMath::Random(0.0f, 1.0f);
        vertices[i].random.y = GameMath::Random(0.0f, 1.0f);
    }

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexParticle), particleCount);
}
