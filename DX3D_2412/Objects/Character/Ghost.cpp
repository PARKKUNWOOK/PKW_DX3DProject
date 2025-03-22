#include "Framework.h"

Ghost::Ghost()
{
	SetTag("Ghost_Collider");
	CapsuleLoad();

	model = new ModelAnimator("Ghost");
	model->ReadClip("Ghost_animation", 4);
	model->CreateTexture();
	model->PlayClip(0);
	model->Load();

	model->SetParent(this);
}

Ghost::~Ghost()
{
	delete model;
}

void Ghost::Update()
{
	Control();
	Move();
	Rotate();

	UpdateWorld();
	model->Update();
}

void Ghost::Render()
{
	model->Render();
	Collider::Render();
}

void Ghost::Edit()
{
	model->Edit();
	CapsuleCollider::Edit();
}

void Ghost::Control()
{
	if (KEY->Down(VK_LBUTTON))
	{
		destPos = terrain->Picking();

		int start = aStar->FindCloseNode(localPosition);
		int end = aStar->FindCloseNode(destPos);

		aStar->GetPath(start, end, path);

		Audio::Get()->Play("Pick");
	}
}

void Ghost::Move()
{
	if (path.empty())
	{
		return;
	}

	Vector3 dest = path.back();

	Vector3 direction = dest - GetGlobalPosition();
	direction.y = 0.0f;

	if (direction.Magnitude() < 1.0f)
		path.pop_back();

	velocity = direction.GetNormalized();

	Translate(velocity * moveSpeed * DELTA);

	localPosition.y = terrain->GetHeight(localPosition);
}

void Ghost::Rotate()
{
	Vector3 forward = GetForward();
	Vector3 cross = Vector3::Cross(forward, velocity);

	if (cross.y > 0)
		Transform::Rotate(Vector3::Up(), rotSpeed * DELTA);
	else if (cross.y < 0)
		Transform::Rotate(Vector3::Down(), rotSpeed * DELTA);
}
