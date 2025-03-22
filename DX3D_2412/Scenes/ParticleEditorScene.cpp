#include "Framework.h"
#include "ParticleEditorScene.h"

ParticleEditorScene::ParticleEditorScene()
{
    quad = new Quad(Float2(1, 1));
    quad->GetMaterial()->SetShader(L"Instancing/Particle.hlsl");
    quad->GetMaterial()->SetDiffuseMap(L"Resources/Textures/FX/Snow.png");

    instances.resize(SIZE);
    particleInfos.resize(SIZE);

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(Matrix), SIZE);    

    char path[128];
    GetCurrentDirectoryA(sizeof(path), path);
    projectPath = path;

    Init();
}

ParticleEditorScene::~ParticleEditorScene()
{
    delete quad;
    delete instanceBuffer;
}

void ParticleEditorScene::Update()
{
    lifeTime += DELTA;

    UpdatePhysical();
    UpdateColor();
    quad->UpdateWorld();

    if (lifeTime > data.duration)
    {        
        Init();
    }
}

void ParticleEditorScene::PreRender()
{
}

void ParticleEditorScene::Render()
{
    instanceBuffer->Set(1);

    if (data.isAdditive)
        Environment::Get()->SetAdditive();
    else
        Environment::Get()->SetAlphaBlend(true);

    Environment::Get()->SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);

    quad->RenderInstanced(data.count);

    Environment::Get()->SetAlphaBlend(false);
    Environment::Get()->SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK_ALL);
}

void ParticleEditorScene::PostRender()
{
}

void ParticleEditorScene::GUIRender()
{
    ImGui::Text("ParticleEditor");

    EditTexture();

    ImGui::Checkbox("Loop", &data.isLoop);
    ImGui::Checkbox("Additive", &data.isAdditive);
    ImGui::Checkbox("Billboard", &data.isBillboard);
    ImGui::SliderInt("ParticleCount", (int*)&particleCount, 1, SIZE);
    ImGui::DragFloat("Duration", &data.duration, 0.1f);
    ImGui::ColorEdit4("StartColor", (float*)&data.startColor);
    ImGui::ColorEdit4("EndColor", (float*)&data.endColor);
    ImGui::SliderFloat3("MinStartPos", (float*)&data.minStartPos, -10, 10);
    ImGui::SliderFloat3("MaxStartPos", (float*)&data.maxStartPos, -10, 10);
    ImGui::SliderFloat3("MinVelocity", (float*)&data.minVelocity, -1, 1);
    ImGui::SliderFloat3("MaxVelocity", (float*)&data.maxVelocity, -1, 1);
    ImGui::SliderFloat3("MinAccelation", (float*)&data.minAccelation, -5, 5);
    ImGui::SliderFloat3("MaxAccelation", (float*)&data.maxAccelation, -5, 5);
    ImGui::SliderFloat3("MinStartScale", (float*)&data.minStartScale, 0, 5);
    ImGui::SliderFloat3("MaxStartScale", (float*)&data.maxStartScale, 0, 5);
    ImGui::SliderFloat3("MinEndScale", (float*)&data.minEndScale, 0, 5);
    ImGui::SliderFloat3("MaxEndScale", (float*)&data.maxEndScale, 0, 5);
    ImGui::SliderFloat("MinAngularVelocity", &data.minAngularVelocity, -10, 10);
    ImGui::SliderFloat("MaxAngularVelocity", &data.maxAngularVelocity, -10, 10);
    ImGui::SliderFloat("MinSpeed", &data.minSpeed, 0, data.maxSpeed);
    ImGui::SliderFloat("MaxSpeed", &data.maxSpeed, data.minSpeed, 200);
    ImGui::SliderFloat("MinStartTime", &data.minStartTime, 0, data.maxStartTime);
    ImGui::SliderFloat("MaxStartTime", &data.maxStartTime, data.minStartTime, data.duration);

    SaveDialog();
    ImGui::SameLine();
    LoadDialog();
}

void ParticleEditorScene::UpdatePhysical()
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

void ParticleEditorScene::UpdateColor()
{
    float t = lifeTime / data.duration;

    Float4 color;
    color.x = GameMath::Lerp(data.startColor.x, data.endColor.x, t);
    color.y = GameMath::Lerp(data.startColor.y, data.endColor.y, t);
    color.z = GameMath::Lerp(data.startColor.z, data.endColor.z, t);
    color.w = GameMath::Lerp(data.startColor.w, data.endColor.w, t);

    quad->GetMaterial()->GetData()->diffuse = color;
}

void ParticleEditorScene::Init()
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

void ParticleEditorScene::Save(string file)
{
    BinaryWriter* writer = new BinaryWriter(file);

    writer->WString(quad->GetMaterial()->GetDiffuseMap()->GetFile());

    writer->Byte(&data, sizeof(ParticleData));

    delete writer;
}

void ParticleEditorScene::Load(string file)
{
    BinaryReader* reader = new BinaryReader(file);

    wstring textureFile = reader->WString();
    quad->GetMaterial()->SetDiffuseMap(textureFile);

    ParticleData* particleData = new ParticleData();
    reader->Byte((void**)&particleData, sizeof(ParticleData));

    data = *particleData;

    UINT temp = data.count;
    data.count = particleCount;
    particleCount = temp;

    delete reader;
}

void ParticleEditorScene::EditTexture()
{
    void* textureID = quad->GetMaterial()->GetDiffuseMap()->GetSRV();

    if (ImGui::ImageButton("Map", (ImTextureID)textureID, ImVec2(50, 50)))
        DIALOG->OpenDialog("SelectTexture", "SelectTexture", ".png,.jpg,.tga");

    if (DIALOG->Display("SelectTexture"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            file = file.substr(projectPath.size() + 1, file.size());

            quad->GetMaterial()->SetDiffuseMap(Utility::ToWString(file));
        }

        DIALOG->Close();
    }
}

void ParticleEditorScene::SaveDialog()
{
    if (ImGui::Button("Save"))
        DIALOG->OpenDialog("Save", "Save", ".fx");

    if (DIALOG->Display("Save"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            file = file.substr(projectPath.size() + 1, file.size());

            Save(file);
        }

        DIALOG->Close();
    }
}

void ParticleEditorScene::LoadDialog()
{
    if (ImGui::Button("Load"))
        DIALOG->OpenDialog("Load", "Load", ".fx");

    if (DIALOG->Display("Load"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            file = file.substr(projectPath.size() + 1, file.size());

            Load(file);
        }

        DIALOG->Close();
    }
}
