#include "Framework.h"

ParticleSystem::ParticleSystem(string file) : Quad(Vector2(1, 1))
{
    Load(file);

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(Matrix), data.count);
}

ParticleSystem::~ParticleSystem()
{
    delete instanceBuffer;
}

void ParticleSystem::Update()
{
    if (!IsActive()) return;

    lifeTime += DELTA;

    UpdatePhysical();
    UpdateColor();
    UpdateWorld();

    if (lifeTime > data.duration)
    {
        if (data.isLoop)
            Init();
        else
            Stop();
    }
}

void ParticleSystem::Render()
{
    if (!IsActive()) return;

    instanceBuffer->Set(1);
    
    if (data.isAdditive)
        Environment::Get()->SetAdditive();
    else
        Environment::Get()->SetAlphaBlend(true);

    RenderInstanced(data.count);

    Environment::Get()->SetAlphaBlend(false);
    Environment::Get()->SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK_ALL);
}

void ParticleSystem::Play(Vector3 pos, Vector3 rot)
{
    SetActive(true);

    SetLocalPosition(pos);
    SetLocalRotation(rot);

    Init();
}

void ParticleSystem::Stop()
{
    SetActive(false);
}

void ParticleSystem::UpdatePhysical()
{
    drawCount = 0;

    FOR(data.count)
    {
        if (particleInfos[i].startTime > lifeTime) continue;

        particleInfos[i].velocity += particleInfos[i].accelation * DELTA;
        particleInfos[i].transform.Translate(
            particleInfos[i].velocity * particleInfos[i].speed * DELTA);
        particleInfos[i].transform.Rotate(
            Vector3::Forward(), particleInfos[i].angularVelocity * DELTA);
        if (data.isBillboard)
        {
            Vector3 rot = CAM->GetLocalRotation();
            rot.z = particleInfos[i].transform.GetLocalRotation().z;
            particleInfos[i].transform.SetLocalRotation(rot);
        }

        float t = (lifeTime - particleInfos[i].startTime)
            / (data.duration - particleInfos[i].startTime);

        Vector3 scale = GameMath::Lerp(particleInfos[i].startScale, particleInfos[i].endScale, t);
        particleInfos[i].transform.SetLocalScale(scale);

        particleInfos[i].transform.UpdateWorld();
        instances[drawCount++] = XMMatrixTranspose(particleInfos[i].transform.GetWorld());
    }

    instanceBuffer->Update(instances.data(), drawCount);
}

void ParticleSystem::UpdateColor()
{
    float t = lifeTime / data.duration;

    Float4 color;
    color.x = GameMath::Lerp(data.startColor.x, data.endColor.x, t);
    color.y = GameMath::Lerp(data.startColor.y, data.endColor.y, t);
    color.z = GameMath::Lerp(data.startColor.z, data.endColor.z, t);
    color.w = GameMath::Lerp(data.startColor.w, data.endColor.w, t);

    material->GetData()->diffuse = color;
}

void ParticleSystem::Init()
{
    lifeTime = 0.0f;
    drawCount = 0;
    data.count = particleCount;

    instances.resize(data.count);
    particleInfos.resize(data.count);

    for (ParticleInfo& info : particleInfos)
    {
        info.transform.SetLocalPosition(GameMath::Random(data.minStartPos, data.maxStartPos));
        info.velocity = GameMath::Random(data.minVelocity, data.maxVelocity);
        info.accelation = GameMath::Random(data.minAccelation, data.maxAccelation);
        info.angularVelocity = GameMath::Random(data.minAngularVelocity, data.maxAngularVelocity);
        info.speed = GameMath::Random(data.minSpeed, data.maxSpeed);
        info.startTime = GameMath::Random(data.minStartTime, data.maxStartTime);
        info.startScale = GameMath::Random(data.minStartScale, data.maxStartScale);
        info.endScale = GameMath::Random(data.minEndScale, data.maxEndScale);
        info.velocity = GameMath::Random(data.minVelocity, data.maxVelocity);
        info.velocity.Normalize();
    }
}

void ParticleSystem::Load(string file)
{
    BinaryReader* reader = new BinaryReader(file);

    if (reader->IsFailed())
        assert(false);

    wstring textureFile = reader->WString();    
    material->SetDiffuseMap(textureFile);
    material->SetShader(L"Instancing/Particle.hlsl");

    ParticleData* particleData = new ParticleData();
    reader->Byte((void**)&particleData, sizeof(ParticleData));

    data = *particleData;

    instances.resize(data.count);
    particleInfos.resize(data.count);

    delete reader;
}
