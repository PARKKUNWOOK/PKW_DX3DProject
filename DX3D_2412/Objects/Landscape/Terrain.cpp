#include "Framework.h"

Terrain::Terrain()
{
	material->SetShader(L"Landscape/Terrain.hlsl");
	//material->SetDiffuseMap(L"Resources/Textures/Colors/White.png");
	//material->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt2.png");
	material->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt.png");
	//material->SetSpecularMap(L"Resources/Textures/Landscape/Fieldstone_SM.tga");
	//material->SetNormalMap(L"Resources/Textures/Landscape/Fieldstone_NM.tga");

	heightMap = Texture::Add(L"Resources/Textures/HeightMaps/PackMan.png");
	alphaMap = Texture::Add(L"Resources/Textures/HeightMaps/PackManAlpha.png");
	secondMap = Texture::Add(L"Resources/Textures/Landscape/Dirt2.png");

	MakeMesh();
	MakeNormal();
	mesh->CreateMesh();	
}

Terrain::~Terrain()
{
}

void Terrain::Render()
{
	alphaMap->PSSet(10);
	secondMap->PSSet(11);

	GameObject::Render();
}

float Terrain::GetHeight(const Vector3& pos, Vector3* normal)
{
	int x = (int)pos.x;
	int z = (int)pos.z;

	if (x < 0 || x >= width - 1) return 0.0f;
	if (z < 0 || z >= height - 1) return 0.0f;

	UINT index[4];
	index[0] = width * z + x;
	index[2] = width * z + x + 1;
	index[1] = width * (z + 1) + x;	
	index[3] = width * (z + 1) + x + 1;

	vector<VertexType> vertices = mesh->GetVertices();

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].pos;

	float u = pos.x - p[0].x;
	float v = pos.z - p[0].z;

	Vector3 result;

	if (u + v <= 1.0)
	{
		result = ((p[2] - p[0]) * u + (p[1] - p[0]) * v) + p[0];

		if (normal)
		{
			(*normal) = GameMath::PolygonToNormal(p[0], p[1], p[2]);
		}
		return result.y;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = ((p[1] - p[3]) * u + (p[2] - p[3]) * v) + p[3];

		if (normal)
		{
			(*normal) = GameMath::PolygonToNormal(p[2], p[1], p[3]);
		}
		return result.y;
	}
}

Vector3 Terrain::Picking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);

	vector<VertexType> vertices = mesh->GetVertices();

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * z + x + 1;
			index[2] = width * (z + 1) + x;
			index[3] = width * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].pos;

			float distance = 0.0f;
			if (Intersects(ray.origin, ray.direction, p[0], p[1], p[2], distance))
			{
				return ray.origin + ray.direction * distance;
			}

			if (Intersects(ray.origin, ray.direction, p[3], p[1], p[2], distance))
			{
				return ray.origin + ray.direction * distance;
			}
		}
	}

	return Vector3();
}

void Terrain::MakeMesh()
{
	width = (UINT)heightMap->GetSize().x;
	height = (UINT)heightMap->GetSize().y;

	vector<Float4> pixels;
	heightMap->ReadPixels(pixels);

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve(width * height);	

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.pos = {(float)x, 0.0f, (float)z};
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			UINT index = width * z + x;
			vertex.pos.y = pixels[index].x * MAX_HEIGHT;

			vertices.push_back(vertex);
		}
	}

	vector<UINT>& indices = mesh->GetIndices();

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
