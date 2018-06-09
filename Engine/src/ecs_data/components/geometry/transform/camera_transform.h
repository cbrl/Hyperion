#pragma once

#include "component/component.h"


struct CameraTransform final : public Component<CameraTransform> {
	friend class TransformSystem;

public:
	CameraTransform();
	~CameraTransform() = default;


	//----------------------------------------------------------------------------------
	// Getters
	//----------------------------------------------------------------------------------

	handle64 GetParent() const { return parent; }

	// Relative position and rotation
	XMVECTOR XM_CALLCONV GetPosition() const { return position; }
	float2 GetPitchYaw() const { return float2(pitch, yaw); }

	// World space data
	XMVECTOR XM_CALLCONV GetWorldAxisX() const { return right; }
	XMVECTOR XM_CALLCONV GetWorldAxisY() const { return up; }
	XMVECTOR XM_CALLCONV GetWorldAxisZ() const { return forward; }
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
		return XMMatrixTranslationFromVector(position);
	}


	//----------------------------------------------------------------------------------
	// Setters
	//----------------------------------------------------------------------------------

	void SetParent(const handle64 handle) { parent = handle; }

	void SetRotation(const float3& rotation);
	void XM_CALLCONV SetRotation(FXMVECTOR rotation);

	void Rotate(const float3& units);

	void SetPosition(const float3& position);
	void XM_CALLCONV SetPosition(FXMVECTOR position);

	void Move(const float3& units);
	void XM_CALLCONV Move(FXMVECTOR units);


private:
	void RotateX(float units);
	void RotateY(float units);
	void RotateZ(float units);


private:
	handle64 parent;

	XMMATRIX world;

	XMVECTOR position;

	XMVECTOR right;
	XMVECTOR up;
	XMVECTOR forward;

	float pitch;
	float max_pitch;
	float yaw;

	bool needs_update;
	bool updated;
};
