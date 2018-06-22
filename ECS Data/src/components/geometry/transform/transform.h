#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "component/component.h"


class Transform final : public Component<Transform> {
	friend class TransformSystem;

public:
	Transform()
		: parent(handle64::invalid_handle)
		, world(XMMatrixIdentity())
		, translation(XMVectorZero())
		, rotation(XMVectorZero())
		, scaling(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f))
		, needs_update(true)
		, updated(false) {
	}

	~Transform() = default;


	bool isUpdated() const {
		return updated;
	}


	//----------------------------------------------------------------------------------
	// Parent
	//----------------------------------------------------------------------------------

	// Set an entity as the parent of this transform. This transform will
	// now be relative to the parent's transform.
	void setParent(const handle64 handle) {
		parent = handle;
	}

	// Get the entity whose transform is a parent of this transform
	[[nodiscard]]
	handle64 getParent() const {
		return parent;
	}


	//----------------------------------------------------------------------------------
	// Position
	//----------------------------------------------------------------------------------

	void move(const float3& move) {
		translation += XMLoadFloat3(&move);
		needs_update = true;
	}

	void XM_CALLCONV move(FXMVECTOR move) {
		translation += move;
		needs_update = true;
	}

	void setPosition(const float3& position) {
		translation = XMLoadFloat3(&position);
		needs_update = true;
	}

	void XM_CALLCONV setPosition(FXMVECTOR position) {
		translation = position;
		needs_update = true;
	}


	//----------------------------------------------------------------------------------
	// Rotation
	//----------------------------------------------------------------------------------

	void rotate(const float3& rotate) {
		rotation += XMLoadFloat3(&rotate);
		needs_update = true;
	}

	void XM_CALLCONV rotate(FXMVECTOR rotate) {
		rotation += rotate;
		needs_update = true;
	}

	void setRotation(const float3& rotation) {
		this->rotation = XMLoadFloat3(&rotation);
		needs_update = true;
	}

	void XM_CALLCONV setRotation(FXMVECTOR rotation) {
		this->rotation = rotation;
		needs_update = true;
	}


	//----------------------------------------------------------------------------------
	// Scale
	//----------------------------------------------------------------------------------

	void scale(const float3& scale) {
		this->scaling *= XMLoadFloat3(&scale);
		needs_update = true;
	}

	void XM_CALLCONV scale(FXMVECTOR scale) {
		this->scaling *= scale;
		needs_update = true;
	}

	void setScale(const float3& scale) {
		this->scaling = XMLoadFloat3(&scale);
		needs_update = true;
	}

	void XM_CALLCONV setScale(FXMVECTOR scale) {
		this->scaling = scale;
		needs_update = true;
	}


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
