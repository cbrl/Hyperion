#pragma once

#include "directx/directx_math.h"
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

	void moveX(f32 units) {
		translation += {units, 0.0f, 0.0f};
		needs_update = true;
	}

	void moveY(f32 units) {
		translation += {0.0f, units, 0.0f};
		needs_update = true;
	}

	void moveZ(f32 units) {
		translation += {0.0f, 0.0f, units};
		needs_update = true;
	}

	void move(const vec3_f32& units) {
		translation += XMLoad(&units);
		needs_update = true;
	}

	void XM_CALLCONV move(FXMVECTOR units) {
		translation += units;
		needs_update = true;
	}

	void setPosition(const vec3_f32& position) {
		translation = XMLoad(&position);
		needs_update = true;
	}

	void XM_CALLCONV setPosition(FXMVECTOR position) {
		translation = position;
		needs_update = true;
	}


	//----------------------------------------------------------------------------------
	// Rotation
	//----------------------------------------------------------------------------------

	void rotateX(f32 units) {
		rotation += {units, 0.0f, 0.0f};
		needs_update = true;
	}

	void rotateY(f32 units) {
		rotation += {0.0f, units, 0.0f};
		needs_update = true;
	}

	void rotateZ(f32 units) {
		rotation += {0.0f, 0.0f, units};
		needs_update = true;
	}

	void rotate(const vec3_f32& units) {
		rotation += XMLoad(&units);
		needs_update = true;
	}

	void XM_CALLCONV rotate(FXMVECTOR units) {
		rotation += units;
		needs_update = true;
	}

	void setRotation(const vec3_f32& rotation) {
		this->rotation = XMLoad(&rotation);
		needs_update = true;
	}

	void XM_CALLCONV setRotation(FXMVECTOR rotation) {
		this->rotation = rotation;
		needs_update = true;
	}


	//----------------------------------------------------------------------------------
	// Scale
	//----------------------------------------------------------------------------------

	void scaleX(f32 units) {
		scaling += {units, 0.0f, 0.0f};
		needs_update = true;
	}

	void scaleY(f32 units) {
		scaling += {0.0f, units, 0.0f};
		needs_update = true;
	}

	void scaleZ(f32 units) {
		scaling += {0.0f, 0.0f, units};
		needs_update = true;
	}

	void scale(const vec3_f32& units) {
		scaling *= XMLoad(&units);
		needs_update = true;
	}

	void XM_CALLCONV scale(FXMVECTOR units) {
		scaling *= units;
		needs_update = true;
	}

	void setScale(const vec3_f32& scale) {
		scaling = XMLoad(&scale);
		needs_update = true;
	}

	void XM_CALLCONV setScale(FXMVECTOR scale) {
		scaling = scale;
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
