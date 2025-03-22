#pragma once

class Spark : public Particle
{
private:
    struct VertexParticle
    {
        Float3 position = {};
        Float2 size = {};
        Float3 velocity = {};
    };

public:
    Spark(wstring imageFile, bool isAdditive = false);
    ~Spark();

    void Update();
    void Render();
    void GUIRender();

    void Play(Vector3 position);

private:
    void Create();
    void UpdateParticle();

private:
    FloatValueBuffer* buffer;
    FloatValueBuffer* startColorbuffer;
    FloatValueBuffer* endColorbuffer;

    float minRadius = 5.0f;
    float maxRadius = 10.0f;

    float minSize = 0.1f;
    float maxSize = 1.0f;    

    vector<VertexParticle> vertices;
};