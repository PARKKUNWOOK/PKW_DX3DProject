#include "Framework.h"

Sprite::Sprite(wstring imageFile, float width, float height, float frameCol, float frameRow, bool isAdditive)
    : size(width, height)
{
    material->SetShader(L"Geometry/Sprite.hlsl");
    material->SetDiffuseMap(imageFile);

    geometryShader = Shader::AddGS(L"Geometry/Sprite.hlsl");

    buffer = new SpriteBuffer();
    buffer->Get().maxFrame = { frameCol, frameRow };

    maxFrameCount = frameCol * frameRow;    

    this->isAdditive = isAdditive;

    Create();
}

Sprite::~Sprite()
{
    delete buffer;
}

void Sprite::Update()
{
    if (!isActive) return;

    time += speed * DELTA;

    if (time > 0.1f)
    {
        curFrameCount++;

        buffer->Get().curFrame.x = curFrameCount % (UINT)buffer->Get().maxFrame.x;
        buffer->Get().curFrame.y = curFrameCount / (UINT)buffer->Get().maxFrame.x;        

        time = 0.0f;
    }

    if (curFrameCount >= maxFrameCount)
        Stop();
}

void Sprite::Render()
{
    buffer->SetGS(10);

    Particle::Render();
}

void Sprite::GUIRender()
{
}

void Sprite::Play(Vector3 position)
{
    isActive = true;
    
    time = 0.099f;
    curFrameCount = (UINT)-1;

    vertex.pos = position;
    vertex.uv = size;

    vertexBuffer->Update(&vertex, particleCount);
}

void Sprite::SetSize(Float2 size)
{
    this->size = size;
    vertex.uv = size;

    vertexBuffer->Update(&vertex, particleCount);
}

void Sprite::Create()
{
    particleCount = 1;

    vertexBuffer = new VertexBuffer(&vertex, sizeof(VertexUV), 1);
}
