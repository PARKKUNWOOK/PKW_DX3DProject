#pragma once

class Water : public GameObject<VertexUV>
{
public:
    Water(wstring normalFile, float width = 100.0f, float height = 100.0f);
    ~Water();

    void Update();

    void Render();
    void GUIRender();

    void SetReflection();
    void SetRefraction();

private:
    void MakeMesh();

private:
    Vector2 size;

    Reflection* reflection;
    Refraction* refraction;
};