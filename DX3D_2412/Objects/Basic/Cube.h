#pragma once

class BoxCollider;

class Cube : public GameObject<VertexUVNormalTangent>
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Cube(Vector3 size = { 1, 1, 1 }, Vector2 tiling = {1, 1});

	void Update();
	void Render();

	BoxCollider* GetCollider() { return collider; }

private:
	void MakeMesh();
	
protected:
	void SetVertices();

protected:
	Vector3 size;	
	Vector2 tiling;

	BoxCollider* collider;
};