#include "Framework.h"

Cone::Cone(float radius, float height, UINT sliceCount)
	: radius(radius), height(height), sliceCount(sliceCount)
{
	SetTag("Cone");

	MakeMesh();
	MakeTangent();
	mesh->CreateMesh();

    collider = new SphereCollider(radius);
    collider->SetParent(this);
    collider->Update();
}

Cone::~Cone()
{
    delete collider;
}

void Cone::Update()
{
    UpdateWorld();
    collider->Update();
}

void Cone::Render()
{
    GameObject::Render();
    collider->Render();
}

void Cone::MakeMesh()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    VertexType apex;
    apex.pos = Float3(0.0f, 0.0f, height);
    apex.normal = Float3(0.0f, 0.0f, 1.0f);
    apex.uv = Float2(0.5f, 1.0f);
    vertices.push_back(apex);

    for (UINT i = 0; i <= sliceCount; ++i)
    {
        float theta = i * XM_2PI / sliceCount;

        VertexType vertex;
        vertex.pos = Float3(cos(theta) * radius, sin(theta) * radius, 0.0f);

        Float3 sideDir = Float3(cos(theta), sin(theta), radius / height);
        float length = sqrt(sideDir.x * sideDir.x + sideDir.y * sideDir.y + sideDir.z * sideDir.z);
        sideDir.x /= length;
        sideDir.y /= length;
        sideDir.z /= length;
        vertex.normal = sideDir;

        vertex.uv = Float2((cos(theta) + 1.0f) * 0.5f, (sin(theta) + 1.0f) * 0.5f);

        vertices.push_back(vertex);
    }

    for (UINT i = 1; i <= sliceCount; ++i)
    {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i % sliceCount + 1);
    }

    VertexType baseCenter;
    baseCenter.pos = Float3(0.0f, 0.0f, 0.0f);
    baseCenter.normal = Float3(0.0f, 0.0f, -1.0f);
    baseCenter.uv = Float2(0.5f, 0.5f);
    vertices.push_back(baseCenter);

    UINT baseCenterIndex = (UINT)vertices.size() - 1;

    for (UINT i = 1; i <= sliceCount; ++i)
    {
        indices.push_back(baseCenterIndex);
        indices.push_back(i % sliceCount + 1);
        indices.push_back(i);
    }
}
