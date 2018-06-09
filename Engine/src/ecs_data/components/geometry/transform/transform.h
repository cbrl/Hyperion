#pragma once

#include <d3d11.h>
#include "datatypes/datatypes.h"
#include "component/component.h"


class Transform final : public Component<Transform> {
	friend class TransformSystem;

public:
	Transform();
	~Transform() = default;

	bool Updated() const { return updated; }


	//----------------------------------------------------------------------------------
	// Setters
	//----------------------------------------------------------------------------------

	void SetParent(const handle64 handle) { parent = handle; }

	void Move(const float3& move);
	void SetPosition(const float3& position);
	void XM_CALLCONV Move(FXMVECTOR move);
	void XM_CALLCONV SetPosition(FXMVECTOR position);

	void Rotate(const float3& rotate);
	void SetRotation(const float3& rotation);
	void XM_CALLCONV Rotate(FXMVECTOR rotate);
	void XM_CALLCONV SetRotation(FXMVECTOR rotation);

	void Scale(const float3& scale);
	void SetScale(const float3& scale);
	void XM_CALLCONV Scale(FXMVECTOR scale);
	void XM_CALLCONV SetScale(FXMVECTOR scale);


	//----------------------------------------------------------------------------------
	// Getters
	//----------------------------------------------------------------------------------

	// Get the entity whose transform is a parent of this transform
	handle64 GetParent() const { return parent; }

	// Position, Rotation, and Scale (relative to parent, if any)
	XMVECTOR XM_CALLCONV GetPosition() const { return translation; }
	XMVECTOR XM_CALLCONV GetRotation() const { return rotation; }
	XMVECTOR XM_CALLCONV GetScale() const { return scale; }

	// World space data
	XMVECTOR XM_CALLCONV GetWorldAxisX() const { return XMVector3Normalize(world.r[0]); }
	XMVECTOR XM_CALLCONV GetWorldAxisY() const { return XMVector3Normalize(world.r[1]); }
	XMVECTOR XM_CALLCONV GetWorldAxisZ() const { return XMVector3Normalize(world.r[2]); }
	XMVECTOR XM_CALLCONV GetWorldOrigin() const { return world.r[3]; }

	// Object space data
	static XMVECTOR XM_CALLCONV GetObjectAxisX() { return XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); }
	static XMVECTOR XM_CALLCONV GetObjectAxisY() { return XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); }
	static XMVECTOR XM_CALLCONV GetObjectAxisZ() { return XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); }
	static XMVECTOR XM_CALLCONV GetObjectOrigin() { return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); }

	// Matrices
	XMMATRIX XM_CALLCONV GetObjectToWorldMatrix() const { return world; }
	XMMATRIX XM_CALLCONV GetWorldToObjectMatrix() const { return XMMatrixInverse(nullptr, world); }

	XMMATRIX XM_CALLCONV GetObjectToParentPositionMatrix() const {
		return XMMatrixTranslationFromVector(translation);
	}

	XMMATRIX XM_CALLCONV GetObjectToParentRotationMatrix() const {
		return XMMatrixRotationRollPitchYawFromVector(rotation);
	}

	XMMATRIX XM_CALLCONV GetObjectToParentScaleMatrix() const {
		return XMMatrixScalingFromVector(scale);
	}


private:
	// The parent of the entity this transform is assigned to (optional)
	handle64 parent;

	// The object-to-world matrix
	XMMATRIX world;

	// Translation, rotation, and scale vectors
	XMVECTOR translation;
	XMVECTOR rotation;
	XMVECTOR scale;

	// Flag that decides if the object-to-world matrix requires an update
	bool needs_update;

	// Has the transform been updated in the past fame?
	bool updated;
};
