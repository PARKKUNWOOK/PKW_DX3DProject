#include "Framework.h"

Pyramid::Pyramid(float baseSize, float height)
	: baseSize(baseSize), height(height)
{
    SetTag("Pyramid");

	MakeMesh();
    MakeTangent();
    mesh->CreateMesh();

    collider = new SphereCollider(baseSize);
    collider->SetParent(this);
    collider->Update();
}

Pyramid::~Pyramid()
{
    delete collider;
}

void Pyramid::Update()
{
    UpdateWorld();
    collider->Update();
}

void Pyramid::Render()
{
    GameObject::Render();
    collider->Render();
}

Float3 Pyramid::CalculateNormal(const Float3& p1, const Float3& p2, const Float3& p3)
{
    Float3 u = Float3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
    Float3 v = Float3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);

    Float3 normal;
    normal.x = (u.y * v.z) - (u.z * v.y);
    normal.y = (u.z * v.x) - (u.x * v.z);
    normal.z = (u.x * v.y) - (u.y * v.x);

    float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x /= length;
    normal.y /= length;
    normal.z /= length;

    return normal;
}

void Pyramid::MakeMesh()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    vertices.clear();
    indices.clear();

    float halfSize = baseSize / 2.0f;

    VertexType apex;
    apex.pos = Float3(0.0f, 0.0f, height);
    apex.uv = Float2(0.5f, 1.0f);
    vertices.push_back(apex);

    VertexType v0, v1, v2, v3;
    v0.pos = Float3(-halfSize, -halfSize, 0.0f);
    v1.pos = Float3(halfSize, -halfSize, 0.0f);
    v2.pos = Float3(halfSize, halfSize, 0.0f);
    v3.pos = Float3(-halfSize, halfSize, 0.0f);

    v0.normal = Float3(0.0f, 0.0f, -1.0f);
    v1.normal = Float3(0.0f, 0.0f, -1.0f);
    v2.normal = Float3(0.0f, 0.0f, -1.0f);
    v3.normal = Float3(0.0f, 0.0f, -1.0f);

    v0.uv = Float2(0.0f, 0.0f);
    v1.uv = Float2(1.0f, 0.0f);
    v2.uv = Float2(1.0f, 1.0f);
    v3.uv = Float2(0.0f, 1.0f);

    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(1);

    indices.push_back(4);
    indices.push_back(3);
    indices.push_back(1);

    for (UINT i = 1; i <= 4; ++i)
    {
        UINT next = (i % 4) + 1;

        Float3 normal = CalculateNormal(apex.pos, vertices[i].pos, vertices[next].pos);

        VertexType vApex = apex;
        vApex.normal = normal;

        VertexType v1 = vertices[i];
        v1.normal = normal;

        VertexType v2 = vertices[next];
        v2.normal = normal;

        UINT apexIndex = vertices.size();
        vertices.push_back(vApex);
        vertices.push_back(v1);
        vertices.push_back(v2);

        indices.push_back(apexIndex);
        indices.push_back(apexIndex + 1);
        indices.push_back(apexIndex + 2);
    }
}
