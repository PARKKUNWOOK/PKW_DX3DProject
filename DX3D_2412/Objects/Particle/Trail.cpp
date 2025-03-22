#include "Framework.h"

Trail::Trail(wstring imageFile, Transform* start, Transform* end,
    UINT width, float frameX, float frameY)
    : start(start), end(end), width(width)
{
    material->SetShader(L"FX/Sprite.hlsl");
    material->SetDiffuseMap(imageFile);

    CreateMesh();

    rasterizerState[0] = new RasterizerState();
    rasterizerState[1] = new RasterizerState();
    rasterizerState[1]->CullMode(D3D11_CULL_NONE);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->Additive();

    frameBuffer = new FloatValueBuffer();
    frameBuffer->GetData()[0] = frameX;
    frameBuffer->GetData()[1] = frameY;

    maxFrame.x = (int)frameX;
    maxFrame.y = (int)frameY;
}

Trail::~Trail()
{
    delete mesh;

    delete rasterizerState[0];
    delete rasterizerState[1];

    delete blendState[0];
    delete blendState[1];
}

void Trail::Update()
{
    if (!IsActive()) return;

    vector<VertexUV>& vertices = mesh->GetVertices();

    FOR(width + 1)
    {
        Vector3 startPos = vertices[i * 2].pos;
        Vector3 endPos = vertices[(i * 2) + 1].pos;

        Vector3 startDestPos;
        Vector3 endDestPos;

        if (i == 0)
        {
            startDestPos = start->GetGlobalPosition();
            endDestPos = end->GetGlobalPosition();

            startPos = startDestPos;
            endPos = endDestPos;
        }
        else
        {
            startDestPos = vertices[(i - 1) * 2].pos;
            endDestPos = vertices[(i - 1) * 2 + 1].pos;
        }

        startPos = MATH->Lerp(startPos, startDestPos, speed * DELTA);
        endPos = MATH->Lerp(endPos, endDestPos, speed * DELTA);

        vertices[i * 2].pos = startPos;
        vertices[i * 2 + 1].pos = endPos;
    }

    mesh->UpdateVertices();

    UpdateFrame();
}

void Trail::Render()
{
    if (!IsActive()) return;

    frameBuffer->SetPS(10);
    SetRender();

    rasterizerState[1]->SetState();
    blendState[1]->SetState();

    mesh->Draw();

    rasterizerState[0]->SetState();
    blendState[0]->SetState();
}

void Trail::Init()
{
    vector<VertexUV>& vertices = mesh->GetVertices();

    FOR(width + 1)
    {
        vertices[i * 2].pos = start->GetGlobalPosition();        
        vertices[(i * 2) + 1].pos = end->GetGlobalPosition();        
    }

    mesh->UpdateVertices();
}

void Trail::CreateMesh()
{
    mesh = new Mesh<VertexUV>();
    
    vector<VertexUV>& vertices = mesh->GetVertices();

    vertices.reserve((width + 1) * 2);

    FOR(width + 1)
    {
        VertexUV vertex;
        vertex.uv = {(float)i / (float)width, 0.0f};
        vertices.push_back(vertex);

        vertex.uv = {(float)i / (float)width, 1.0f};
        vertices.push_back(vertex);
    }

    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve(width * 6);
    FOR(width)
    {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 2);
        indices.push_back(i * 2 + 1);

        indices.push_back(i * 2 + 1);
        indices.push_back(i * 2 + 2);
        indices.push_back(i * 2 + 3);
    }

    mesh->CreateMesh();
}

void Trail::UpdateFrame()
{
    time += DELTA;

    if (time >= FRAME_INTERVAL)
    {
        time -= FRAME_INTERVAL;

        curFrame = ++curFrame % (maxFrame.x * maxFrame.y);

        frameBuffer->GetData()[2] = curFrame % maxFrame.x;
        frameBuffer->GetData()[3] = curFrame / maxFrame.x;        
    }
}
