#pragma once

class Sprite : public Particle
{
public:
    Sprite(wstring imageFile, float width, float height,
        float frameCol, float frameRow, bool isAdditive = false);
    ~Sprite();

    void Update();
    void Render();
    void GUIRender();

    void Play(Vector3 position);

    void SetSize(Float2 size);

    bool IsActive() const;

private:
    void Create();

private:
    SpriteBuffer* buffer;
    VertexUV vertex;

    float time = 0.0f, speed = 1.0f;
    UINT curFrameCount = 0, maxFrameCount = 0;
    Float2 size;
};