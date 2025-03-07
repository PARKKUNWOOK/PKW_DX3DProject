#pragma once

class SphereCollider;

class Cone : public GameObject<VertexUVNormalTangent>
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Cone(float radius = 1.0f, float height = 2.0f, UINT sliceCount = 36);
	~Cone();

	void Update();
	void Render();

	SphereCollider* GetCollider() { return collider; }

private:
	void MakeMesh();

private:
	float radius;
	float height;
	UINT sliceCount;

	SphereCollider* collider;
};