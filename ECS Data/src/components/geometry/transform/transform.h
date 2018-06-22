#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "component/component.h"


class Transform final : public Component<Transform> {
	friend class TransformSystem;

public:
	Transform();
	~Transform() = default;

	bool isUpdated() const { return updated; }


	//----------------------------------------------------------------------------------
	// Setters
	//----------------------------------------------------------------------------------

	void setParent(const handle64 handle) { parent = handle; }

	void move(const float3& move);
	void setPosition(const float3& position);
	void XM_CALLCONV move(FXMVECTOR move);
	void XM_CALLCONV setPosition(FXMVECTOR position);

	void rotate(const float3& rotate);
	void setRotation(const float3& rotation);
	void XM_CALLCONV rotate(FXMVECTOR rotate);
	void XM_CALLCONV setRotation(FXMVECTOR rotation);

	void scale(const float3& scale);
	void setScale(const float3& scale);
	void XM_CALLCONV scale(FXMVECTOR scale);
	void XM_CALLCONV setScale(FXMVECTOR scale);


	// Get the entity whose transform is a parent of this transform
	[[nodiscard]]
	handle64 getParent() const { return parent; }


	//----------------------------------------------------------------------------------
	// Position, Rotation, and Scale (relative to parent, if any)
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	XMVECTOR XM_CALLCONV getPosition() const {
		return translation;
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getRotation() const {
		return rotation;
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getScale() const {
		return scaling;
	}


	//----------------------------------------------------------------------------------
	// World space data
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisX() const {
		return XMVector3Normalize(world.r[0]);
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisY() const {
		return XMVector3Normalize(world.r[1]);
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisZ() const {
		return XMVector3Normalize(world.r[2]);
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
		return XMMatrixTranslationFromVector(translation);
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentRotationMatrix() const {
		return XMMatrixRotationRollPitchYawFromVector(rotation);
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentScaleMatrix() const {
		return XMMatrixScalingFromVector(scaling);
	}


private:
	// The parent of the entity this transform is assigned to (optional)
	handle64 parent;

	// The object-to-world matrix
	XMMATRIX world;

	// Translation, rotation, and scale vectors
	XMVECTOR translation;
	XMVECTOR rotation;
	XMVECTOR scaling;

	// Flag that decides if the object-to-world matrix requires an update
	bool needs_update;

	// Has the transform been updated in the past fame?
	bool updated;
};
