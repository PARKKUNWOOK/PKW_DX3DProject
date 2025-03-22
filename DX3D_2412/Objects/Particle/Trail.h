#pragma once

class Trail : public GameObject
{
private:
    const float FRAME_INTERVAL = 0.05f;

public:
    Trail(wstring imageFile, Transform* start, Transform* end,
        UINT width, float frameX = 1, float frameY = 1);
    ~Trail();

    void Update();
    void Render();

    void Init();

    void SetSpeed(float speed) { this->speed = speed; }    

private:
    void CreateMesh();
    void UpdateFrame();

private:
    Mesh<VertexUV>* mesh;

    Transform* start, *end;

    UINT width;
    float speed = 1.0f;

    float time = 0.0f;
    UINT curFrame = 0;
    POINT maxFrame;

    RasterizerState* rasterizerState[2];
    BlendState* blendState[2];

    FloatValueBuffer* frameBuffer;
};