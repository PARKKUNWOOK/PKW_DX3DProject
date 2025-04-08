#include "Framework.h"

Transform::Transform()
{
	world = XMMatrixIdentity();
}

void Transform::Edit()
{
    if (ImGui::TreeNode((tag + "_Transform").c_str()))
    {
        ImGui::Checkbox("Active", &isActive);

        ImGui::DragFloat3("Pos", (float*)&localPosition, 0.1f);

        Float3 rot;
        rot.x = XMConvertToDegrees(localRotation.x);
        rot.y = XMConvertToDegrees(localRotation.y);
        rot.z = XMConvertToDegrees(localRotation.z);

        if (ImGui::DragFloat3("Rot", (float*)&rot, 0.1f, -180, 180))
        {
            localRotation.x = XMConvertToRadians(rot.x);
            localRotation.y = XMConvertToRadians(rot.y);
            localRotation.z = XMConvertToRadians(rot.z);
        }

        ImGui::DragFloat3("Scale", (float*)&localScale, 0.1f);
        ImGui::DragFloat3("Pivot", (float*)&pivot, 0.1f);

        if (ImGui::Button("Save"))
            Save();
        if (ImGui::Button("Load"))
            Load();

        ImGui::TreePop();
    }
}

void Transform::Save()
{
    BinaryWriter* writer = new BinaryWriter("Resources/Transforms/" + tag + ".srt");

    writer->Data<bool>(isActive);
    writer->Data<Vector3>(localPosition);
    writer->Data<Vector3>(localRotation);
    writer->Data<Vector3>(localScale);
    writer->Data<Vector3>(pivot);

    delete writer;
}

void Transform::Load()
{
    BinaryReader* reader = new BinaryReader("Resources/Transforms/" + tag + ".srt");

    if (reader->IsFailed())
    {
        delete reader;
        return;
    }

    isActive = reader->Data<bool>();
    localPosition = reader->Data<Vector3>();
    localRotation = reader->Data<Vector3>();
    localScale = reader->Data<Vector3>();
    pivot = reader->Data<Vector3>();

    delete reader;

    UpdateWorld();
}

void Transform::UpdateWorld()
{
	world = XMMatrixTransformation(pivot, XMQuaternionIdentity(),
		localScale, pivot, XMQuaternionRotationRollPitchYawFromVector(localRotation),
		localPosition);

	if (parent)
		world *= parent->world;

	XMStoreFloat4x4(&matWorld, world);
	right = Vector3(matWorld._11, matWorld._12, matWorld._13);
	up = Vector3(matWorld._21, matWorld._22, matWorld._23);
	forward = Vector3(matWorld._31, matWorld._32, matWorld._33);

	XMMatrixDecompose(globalScale.Get(), globalRotation.Get(), globalPosition.Get(), world);
}

void Transform::SetGlobalPosition(const Vector3& position)
{
    localPosition = position;

    if (parent)
        localPosition -= parent->GetGlobalPosition();
}

void Transform::SetGlobalRotation(const Vector3& rotation)
{
    Matrix worldRotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

    if (parent)
    {
        // 2️⃣ 부모의 월드 회전 행렬을 가져와서 역행렬을 구함
        Matrix parentWorldRotationMatrix = XMMatrixRotationRollPitchYawFromVector(parent->GetGlobalRotation());
        Matrix invParentRotationMatrix = XMMatrixInverse(nullptr, parentWorldRotationMatrix);

        // 3️⃣ 로컬 회전 행렬 계산 (부모 역행렬을 곱하여 보정)
        Matrix localRotationMatrix = invParentRotationMatrix * worldRotationMatrix;

        XMFLOAT4X4 localRotationFloat4x4;
        XMStoreFloat4x4(&localRotationFloat4x4, localRotationMatrix);

        // 5️⃣ 라디안 값을 계산하여 localRotation에 저장
        localRotation.x = atan2(localRotationFloat4x4._32, localRotationFloat4x4._33);
        localRotation.y = asin(-localRotationFloat4x4._31);
        localRotation.z = atan2(localRotationFloat4x4._21, localRotationFloat4x4._11);
    }
    else
    {     
        localRotation = rotation;
    }    
}

bool Transform::IsActive()
{
    if (parent == nullptr)
        return isActive;

    if (isActive == false)
        return false;

    return parent->IsActive();
}
