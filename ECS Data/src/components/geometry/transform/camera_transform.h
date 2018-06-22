#pragma once

#include "component/component.h"


struct CameraTransform final : public Component<CameraTransform> {
	friend class TransformSystem;

public:
	CameraTransform();
	~CameraTransform() = default;


	// Get the entity whose transform is a parent of this transform
	[[nodiscard]]
	handle64 getParent() const {
		return parent;
	}


	//----------------------------------------------------------------------------------
	// Relative position and rotation
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	XMVECTOR XM_CALLCONV getPosition() const {
		return position;
	}

	[[nodiscard]]
	float2 getPitchYaw() const {
		return float2{ pitch, yaw };
	}


	//----------------------------------------------------------------------------------
	// World space data
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisX() const {
		return right;
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisY() const {
		return up;
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisZ() const {
		return forward;
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldOrigin() const {
		return world.r[3];
	}


	//----------------------------------------------------------------------------------
	// Object space data
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	static XMVECTOR XM_CALLCONV getObjectAxisX() {
		return XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	}

	[[nodiscard]]
	static XMVECTOR XM_CALLCONV getObjectAxisY() {
		return XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	}

	[[nodiscard]]
	static XMVECTOR XM_CALLCONV getObjectAxisZ() {
		return XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	}

	[[nodiscard]]
	static XMVECTOR XM_CALLCONV getObjectOrigin() {
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}


	//----------------------------------------------------------------------------------
	// Matrices
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToWorldMatrix() const {
		return world;
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getWorldToObjectMatrix() const {
		return XMMatrixInverse(nullptr, world);
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentPositionMatrix() const {
		return XMMatrixTranslationFromVector(position);
	}


	//----------------------------------------------------------------------------------
	// Setters
	//----------------------------------------------------------------------------------

	void setParent(const handle64 handle) { parent = handle; }

	void setRotation(const float3& rotation);
	void XM_CALLCONV setRotation(FXMVECTOR rotation);

	void rotate(const float3& units);

	void setPosition(const float3& position);
	void XM_CALLCONV setPosition(FXMVECTOR position);

	void move(const float3& units);
	void XM_CALLCONV move(FXMVECTOR units);


private:
	void rotateX(float units);
	void rotateY(float units);
	void rotateZ(float units);


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
