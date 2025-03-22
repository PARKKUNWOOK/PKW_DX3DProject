#pragma once

class Terrain : public GameObject<VertexUVNormal>
{
private:
	typedef VertexUVNormal VertexType;
	const float MAX_HEIGHT = 20.0f;

public:
	Terrain();
	~Terrain();

	void Render();

	float GetHeight(const Vector3& pos, Vector3* normal = nullptr);
	Vector3 Picking();

	Vector2 GetSize() { return Vector2(width, height); }
private:
	void MakeMesh();	

private:
	UINT width, height;

	Texture* heightMap;
	Texture* alphaMap;
	Texture* secondMap;
};
