#include "Framework.h"

Rain::Rain()
{
    material->SetShader(L"Geometry/Rain.hlsl");
    material->SetDiffuseMap(L"Resources/Textures/FX/Rain.png");

    geometryShader = Shader::AddGS(L"Geometry/Rain.hlsl");

    buffer = new WeatherBuffer();    

    Create();
}

Rain::~Rain()
{
    delete buffer;
}

void Rain::Update()
{
    buffer->GetData()->time += DELTA;
}

void Rain::Render()
{
    buffer->SetVS(10);
    Particle::Render();
}

void Rain::GUIRender()
{
    ImGui::Text("Rain Option");
    ImGui::DragFloat3("Velocity", (float*)&buffer->GetData()->velocity, 0.1f);
    ImGui::SliderFloat("Distance", (float*)&buffer->GetData()->distance, 0.0f, 500.0f);
    ImGui::SliderFloat("Turbulence", (float*)&buffer->GetData()->turbulence, 0.0f, 10.0f);
    ImGui::DragFloat3("Origin", (float*)&buffer->GetData()->origin, 0.1f);
    ImGui::DragFloat3("Size", (float*)&buffer->GetData()->size, 0.1f);
    ImGui::ColorEdit3("Color", (float*)&buffer->GetData()->color);
}

void Rain::Create()
{
    particleCount = SIZE;

    vertices.resize(particleCount);

    FOR(particleCount)
    {
        Float2 size;
        size.x = GameMath::Random(0.1f, 0.4);
        size.y = GameMath::Random(4.0f, 8.0f);

        Float3 pos;
        pos.x = GameMath::Random(-buffer->GetData()->size.x, buffer->GetData()->size.x);
        pos.y = GameMath::Random(-buffer->GetData()->size.y, buffer->GetData()->size.y);
        pos.z = GameMath::Random(-buffer->GetData()->size.z, buffer->GetData()->size.z);

        vertices[i].pos = pos;
        vertices[i].uv = size;
    }

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexUV), particleCount);
}
