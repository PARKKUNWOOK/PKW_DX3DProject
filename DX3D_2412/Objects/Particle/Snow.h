#pragma once

class Snow : public Particle
{
protected:
    struct VertexParticle
    {
        Float3 pos = {};
        Float2 size = {};
        Float2 random = {};
    };

public:
    Snow();
    ~Snow();

    void Update();
    void Render();
    void GUIRender();

    void SetActive(bool active);
    bool IsActive() const { return isActive; }

protected:
    void Create();

protected:
    WeatherBuffer* buffer;
    vector<VertexParticle> vertices;
};