#pragma once

class Ghost : public CapsuleCollider
{
public:
	Ghost();
	~Ghost();

	void Update();
	void Render();
	void Edit();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetAStar(AStar* aStar) { this->aStar = aStar; }

private:
	void Control();
	void Move();
	void Rotate();

private:
	ModelAnimator* model;

	float moveSpeed = 10.0f;
	float rotSpeed = 10.0f;

	Vector3 velocity;

	Vector3 destPos;
	vector<Vector3> path;

	Terrain* terrain;
	AStar* aStar;
};