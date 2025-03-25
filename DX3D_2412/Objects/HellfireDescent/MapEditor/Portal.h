#pragma once

class Portal : public Snow
{
public:
	Portal(Vector3 size = {1, 1, 1});
	~Portal();

	void Update();
	void Render();
	void GUIRender();

	void SetLocalPosition(const Vector3& pos);
	void SetLocalScale(const Vector3& scale);
	Vector3 GetLocalPosition();
	Vector3 GetLocalScale();
	void SetActive(bool active);
	bool IsActive();

	BoxCollider* GetCollider() { return collider; }

private:
	BoxCollider* collider;
};