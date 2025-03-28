#include "Framework.h"

TerrainEditor::TerrainEditor() 
    : width(100), height(100)
{
    material->SetShader(L"Landscape/TerrainEditor.hlsl");
	material->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt2.png");

	//heightMap = Texture::Add(L"Resources/Textures/HeightMaps/AlphaMap.png");

    secondMap = Texture::Add(L"Resources/Textures/Landscape/Stones.png");
    thirdMap = Texture::Add(L"Resources/Textures/Landscape/Dirt3.png");

	MakeMesh();
	MakeNormal();
    MakeComputeData();
	mesh->CreateMesh();

    brushBuffer = new BrushBuffer();

    //Compute
    computeShader = Shader::AddCS(L"Compute/ComputePicking.hlsl");
    structuredBuffer = new StructuredBuffer(
        inputs.data(), sizeof(InputDesc), triangleSize,
        sizeof(OutputDesc), triangleSize);
    rayBuffer = new RayBuffer();

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

TerrainEditor::~TerrainEditor()
{
    delete brushBuffer;
    delete rayBuffer;
    delete structuredBuffer;
}

void TerrainEditor::Update()
{
    if (ComputePicking(pickingPos))
    {
        brushBuffer->Get().pickingPos = pickingPos;
    }

    if (KEY->Press(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
    {
        switch (editType)
        {
        case TerrainEditor::HEIGHT:
            AdjustHeight();
            break;
        case TerrainEditor::ALPHA:
            AdjustAlpha();
            break;
        }
    }

    if (KEY->Up(VK_LBUTTON))
    {
        UpdateHeight();
    }
}

void TerrainEditor::Render()
{
    brushBuffer->SetPS(10);
    secondMap->PSSet(11);
    thirdMap->PSSet(12);

	GameObject::Render();
}

void TerrainEditor::GUIRender()
{
    ImGui::Text("PickingPos : %f, %f, %f", pickingPos.x, pickingPos.y, pickingPos.z);

    ImGui::DragFloat("BrushRange", &brushBuffer->Get().range);
    ImGui::ColorEdit3("BrushColor", (float*)&brushBuffer->Get().color);

    const char* editList[] = { "Height", "Alpha" };
    ImGui::Combo("EditType", (int*)&editType, editList, 2);

    ImGui::DragFloat("AdjustValue", &adjustValue, 1.0f, -50.0f, 50.0f);
    ImGui::DragInt("SelectMap", (int*)&selectMap, 1.0f, 0, 2);

    SaveHeightMap();
    ImGui::SameLine();
    LoadHeightMap();

    SaveAlphaMap();
    ImGui::SameLine();
    LoadAlphaMap();
}

void TerrainEditor::Picking()
{
    Ray ray = CAM->ScreenPointToRay(mousePos);

    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            UINT index[4];
            index[0] = width * z + x;
            index[1] = width * z + x + 1;
            index[2] = width * (z + 1) + x;
            index[3] = width * (z + 1) + x + 1;

            vector<VertexType>& vertices = mesh->GetVertices();

            Vector3 p[4];
            FOR(4)
                p[i] = vertices[index[i]].pos;

            float distance = 0.0f;

            if (Intersects(ray.origin, ray.direction, p[0], p[1], p[2], distance))
            {
                pickingPos = ray.origin + ray.direction * distance;
                return;
            }
            if (Intersects(ray.origin, ray.direction, p[3], p[1], p[2], distance))
            {
                pickingPos = ray.origin + ray.direction * distance;
                return;
            }
        }
    }
}

bool TerrainEditor::ComputePicking(Vector3& pos)
{
    Ray ray = CAM->ScreenPointToRay(mousePos);

    rayBuffer->Get().pos = ray.origin;
    rayBuffer->Get().dir = ray.direction;
    rayBuffer->Get().triangleSize = triangleSize;

    rayBuffer->SetCS(0);

    DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
    DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

    computeShader->Set();

    UINT x = ceil((float)triangleSize / 64.0f);

    DC->Dispatch(x, 1, 1);

    structuredBuffer->Copy(outputs.data(), sizeof(OutputDesc) * triangleSize);

    float minDistance = FLT_MAX;
    int minIndex = -1;

    UINT index = 0;
    for (OutputDesc output : outputs)
    {
        if (output.picked)
        {
            if (minDistance > output.distance)
            {
                minDistance = output.distance;
                minIndex = index;
            }
        }
        index++;
    }

    if (minIndex >= 0)
    {
        pos = ray.origin + ray.direction * minDistance;
        return true;
    }

    return false;
}

void TerrainEditor::MakeMesh()
{
    vector<Float4> pixels(width * height, Float4(0, 0, 0, 0));

    if (heightMap)
    {
        width = heightMap->GetSize().x;
        height = heightMap->GetSize().y;

        heightMap->ReadPixels(pixels);
    }

    //Vertices
    vector<VertexType>& vertices = mesh->GetVertices();
    vertices.clear();

    vertices.reserve(width * height);
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            VertexType vertex;
            vertex.pos = { (float)x, 0.0f, (float)z };
            vertex.uv.x = x / (float)(width - 1);
            vertex.uv.y = z / (float)(height - 1);

            UINT index = width * z + x;
            vertex.pos.y = pixels[index].x * MAX_HEIGHT;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = mesh->GetIndices();
    indices.clear();

    indices.reserve((width - 1) * (height - 1) * 6);

    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            indices.push_back(width * z + x);//0
            indices.push_back(width * (z + 1) + x);//1
            indices.push_back(width * z + x + 1);//2

            indices.push_back(width * z + x + 1);//2
            indices.push_back(width * (z + 1) + x);//1
            indices.push_back(width * (z + 1) + x + 1);//3
        }
    }
}

void TerrainEditor::MakeComputeData()
{
    vector<VertexType> vertices = mesh->GetVertices();
    vector<UINT> indices = mesh->GetIndices();

    triangleSize = indices.size() / 3;

    inputs.resize(triangleSize);
    outputs.resize(triangleSize);

    for (UINT i = 0; i < triangleSize; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        inputs[i].v0 = vertices[index0].pos;
        inputs[i].v1 = vertices[index1].pos;
        inputs[i].v2 = vertices[index2].pos;
    }
}

void TerrainEditor::AdjustHeight()
{
    vector<VertexType>& vertices = mesh->GetVertices();

    for (VertexType& vertex : vertices)
    {
        Vector3 pos = Vector3(vertex.pos.x, 0, vertex.pos.z);
        pickingPos.y = 0.0f;

        float distance = Vector3::Distance(pos, pickingPos);

        if (distance <= brushBuffer->Get().range)
        {
            vertex.pos.y += adjustValue * DELTA;
            vertex.pos.y = GameMath::Clamp(0.0f, MAX_HEIGHT, vertex.pos.y);
        }
    }

    mesh->UpdateVertices();
}

void TerrainEditor::AdjustAlpha()
{
    vector<VertexType>& vertices = mesh->GetVertices();

    for (VertexType& vertex : vertices)
    {
        Vector3 pos = Vector3(vertex.pos.x, 0, vertex.pos.z);
        pickingPos.y = 0.0f;

        float distance = Vector3::Distance(pos, pickingPos);

        if (distance <= brushBuffer->Get().range)
        {
            vertex.alpha[selectMap] += adjustValue * DELTA;
            vertex.alpha[selectMap] = GameMath::Clamp(0.0f, 1.0f, vertex.alpha[selectMap]);
        }
    }

    mesh->UpdateVertices();
}

void TerrainEditor::SaveHeightMap()
{
    if (ImGui::Button("SaveHeight"))
        DIALOG->OpenDialog("SaveHeight", "SaveHeight", ".png");

    if (DIALOG->Display("SaveHeight"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            UINT size = width * height * 4;
            uint8_t* pixels = new uint8_t[size];

            vector<VertexType> vertices = mesh->GetVertices();

            for (UINT i = 0; i < size / 4; i++)
            {
                float y = vertices[i].pos.y;

                uint8_t height = y / MAX_HEIGHT * 255;

                pixels[i * 4 + 0] = height;
                pixels[i * 4 + 1] = height;
                pixels[i * 4 + 2] = height;
                pixels[i * 4 + 3] = 255;
            }

            Image image;
            image.width = width;
            image.height = height;
            image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
            image.rowPitch = width * 4;
            image.slicePitch = size;
            image.pixels = pixels;

            SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
                GetWICCodec(WIC_CODEC_PNG), Utility::ToWString(file).c_str());

            delete[] pixels;
        }

        DIALOG->Close();
    }
}

void TerrainEditor::LoadHeightMap()
{
    if (ImGui::Button("LoadHeight"))
        DIALOG->OpenDialog("LoadHeight", "LoadHeight", ".png");

    if (DIALOG->Display("LoadHeight"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            heightMap = Texture::Add(Utility::ToWString(file));

            Resize();
        }

        DIALOG->Close();
    }
}

void TerrainEditor::SaveAlphaMap()
{
    if (ImGui::Button("SaveAlpha"))
        DIALOG->OpenDialog("SaveAlpha", "SaveAlpha", ".png");

    if (DIALOG->Display("SaveAlpha"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            UINT size = width * height * 4;
            uint8_t* pixels = new uint8_t[size];

            vector<VertexType> vertices = mesh->GetVertices();

            for (UINT i = 0; i < size / 4; i++)
            {
                pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
                pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
                pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
                pixels[i * 4 + 3] = 255;
            }

            Image image;
            image.width = width;
            image.height = height;
            image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
            image.rowPitch = width * 4;
            image.slicePitch = size;
            image.pixels = pixels;

            SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
                GetWICCodec(WIC_CODEC_PNG), Utility::ToWString(file).c_str());

            delete[] pixels;
        }

        DIALOG->Close();
    }
}

void TerrainEditor::LoadAlphaMap()
{
    if (ImGui::Button("LoadAlpha"))
        DIALOG->OpenDialog("LoadAlpha", "LoadAlpha", ".png");

    if (DIALOG->Display("LoadAlpha"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Texture* alphaMap = Texture::Add(Utility::ToWString(file));

            vector<Float4> pixels;
            alphaMap->ReadPixels(pixels);

            vector<VertexType>& vertices = mesh->GetVertices();

            for (UINT i = 0; i < vertices.size(); i++)
            {
                vertices[i].alpha[0] = pixels[i].z;
                vertices[i].alpha[1] = pixels[i].y;
                vertices[i].alpha[2] = pixels[i].x;
                //vertices[i].alpha[3] = pixels[i].w;
            }

            mesh->UpdateVertices();
        }

        DIALOG->Close();
    }
}

void TerrainEditor::Resize()
{
    MakeMesh();
    MakeNormal();    
    MakeComputeData();

    mesh->UpdateVertices();
    mesh->UpdateIndices();
    structuredBuffer->UpdateInput(inputs.data());
}

void TerrainEditor::UpdateHeight()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    for (VertexType& vertex : vertices)
    {
        vertex.normal = {};        
    }

    MakeNormal();    
    MakeComputeData();

    mesh->UpdateVertices();
    structuredBuffer->UpdateInput(inputs.data());
}
