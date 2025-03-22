#include "Framework.h"

Spark::Spark(wstring imageFile, bool isAdditive)
{
    material->SetShader(L"Geometry/Spark.hlsl");
    material->SetDiffuseMap(imageFile);

    geometryShader = Shader::AddGS(L"Geometry/Spark.hlsl");

    buffer = new FloatValueBuffer();    
    startColorbuffer = new FloatValueBuffer();
    endColorbuffer = new FloatValueBuffer();

    this->isAdditive = isAdditive;

    Create();
}

Spark::~Spark()
{
    delete buffer;
}

void Spark::Update()
{
    if (!isActive)return;

    buffer->Get()[0] += DELTA;

    if (buffer->Get()[0] > buffer->Get()[1])
        Stop();
}

void Spark::Render()
{
    buffer->SetVS(10);
    buffer->SetPS(10);
    startColorbuffer->SetPS(11);
    endColorbuffer->SetPS(12);

    Particle::Render();
}

void Spark::GUIRender()
{
    ImGui::Text("Spark Option");
    ImGui::SliderInt("ParticleCount", (int*)&particleCount, 1, SIZE);
    ImGui::ColorEdit4("StartColor", startColorbuffer->Get());
    ImGui::ColorEdit4("EndColor", endColorbuffer->Get());
}

void Spark::Play(Vector3 position)
{
    buffer->Get()[0] = 0.0f;

    Particle::Play(position);

    UpdateParticle();
    vertexBuffer->Update(vertices.data(), particleCount);
}

void Spark::Create()
{
    vertices.resize(SIZE);
    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexParticle), SIZE);

    particleCount = 500;
    buffer->Get()[1] = 1.0f;
}

void Spark::UpdateParticle()
{
    FOR(particleCount)
    {
        vertices[i].position = position;

        float size = GameMath::Random(minSize, maxSize);
        vertices[i].size = { size, size };

        float radius = GameMath::Random(minRadius, maxRadius);

        Float3 rot;
        rot.x = GameMath::Random(0.0f, XM_2PI);
        rot.y = GameMath::Random(0.0f, XM_2PI);
        rot.z = GameMath::Random(0.0f, XM_2PI);

        Vector3 velocity = Vector3(0, 0, radius);

        Matrix matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
        XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity, matRot));
    }
}

