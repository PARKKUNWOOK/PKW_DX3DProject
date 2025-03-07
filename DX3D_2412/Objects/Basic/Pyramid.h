#pragma once

class SphereCollider;

class Pyramid : public GameObject<VertexUVNormalTangent>
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Pyramid(float baseSize = 1.0f, float height = 2.0f);
	~Pyramid();

	void Update();
	void Render();

	SphereCollider* GetCollider() { return collider; }

private:
	void MakeMesh();
	Float3 CalculateNormal(const Float3& p1, const Float3& p2, const Float3& p3);

private:
	float baseSize;
	float height;
	SphereCollider* collider;
};