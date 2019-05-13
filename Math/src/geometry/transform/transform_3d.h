#pragma once

#include "datatypes/scalar_types.h"
#include "directxmath/directxmath.h"


class Transform3D final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Transform3D() noexcept = default;
	Transform3D(const Transform3D& transform) noexcept = default;
	Transform3D(Transform3D&& transform) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Transform3D() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Transform3D& operator=(const Transform3D& transform) noexcept = default;
	Transform3D& operator=(Transform3D&& transform) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------

	void setNeedsUpdate() {
		needs_update = true;
	}

	void updateMatrix(const XMMATRIX* parent = nullptr) const {
		if (needs_update) {
			world = XMMatrixScalingFromVector(getScale())
			        * XMMatrixRotationRollPitchYawFromVector(getRotation())
			        * XMMatrixTranslationFromVector(getPosition());
			if (parent)
				world *= *parent;
			needs_update = false;
		}
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Position
	//----------------------------------------------------------------------------------

	void moveX(f32 units) {
		translation += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		setNeedsUpdate();
	}

	void moveY(f32 units) {
		translation += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		setNeedsUpdate();
	}

	void moveZ(f32 units) {
		translation += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		setNeedsUpdate();
	}

	void move(const f32_3& units) {
		translation += XMLoad(&units);
		setNeedsUpdate();
	}

	void XM_CALLCONV move(FXMVECTOR units) {
		translation += units;
		setNeedsUpdate();
	}

	void setPosition(const f32_3& position) {
		translation = XMLoad(&position);
		setNeedsUpdate();
	}

	void XM_CALLCONV setPosition(FXMVECTOR position) {
		translation = position;
		setNeedsUpdate();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Rotation
	//----------------------------------------------------------------------------------

	void rotateX(f32 units) {
		rotation += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		setNeedsUpdate();
	}

	void rotateY(f32 units) {
		rotation += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		setNeedsUpdate();
	}

	void rotateZ(f32 units) {
		rotation += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		setNeedsUpdate();
	}

	void rotateXClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetX(rotation) + units, min, max);
		rotation = XMVectorSetX(rotation, amount);
		setNeedsUpdate();
	}

	void rotateYClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetY(rotation) + units, min, max);
		rotation = XMVectorSetY(rotation, amount);
		setNeedsUpdate();
	}

	void rotateZClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetZ(rotation) + units, min, max);
		rotation = XMVectorSetZ(rotation, amount);
		setNeedsUpdate();
	}

	void rotate(const f32_3& units) {
		rotation += XMLoad(&units);
		setNeedsUpdate();
	}

	void XM_CALLCONV rotate(FXMVECTOR units) {
		rotation += units;
		setNeedsUpdate();
	}

	void rotateClamped(const f32_3& units, f32 min, f32 max) {
		rotateXClamped(units[0], min, max);
		rotateYClamped(units[1], min, max);
		rotateZClamped(units[2], min, max);
	}

	void XM_CALLCONV rotateClamped(FXMVECTOR units, f32 min, f32 max) {
		rotateXClamped(XMVectorGetX(units), min, max);
		rotateYClamped(XMVectorGetY(units), min, max);
		rotateZClamped(XMVectorGetZ(units), min, max);
	}

	void rotateAround(f32_3 axis, f32 units) {
		rotateAround(XMLoad(&axis), units);
		setNeedsUpdate();
	}

	void XM_CALLCONV rotateAround(FXMVECTOR axis, f32 units) {
		translation = XMVector3Transform(translation, XMMatrixRotationAxis(axis, units));
		setNeedsUpdate();
	}

	void setRotation(const f32_3& rotation) {
		this->rotation = XMLoad(&rotation);
		setNeedsUpdate();
	}

	void XM_CALLCONV setRotation(FXMVECTOR rotation) {
		this->rotation = rotation;
		setNeedsUpdate();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Scale
	//----------------------------------------------------------------------------------

	void scaleX(f32 units) {
		scaling += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		setNeedsUpdate();
	}

	void scaleY(f32 units) {
		scaling += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		setNeedsUpdate();
	}

	void scaleZ(f32 units) {
		scaling += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		setNeedsUpdate();
	}

	void scale(const f32_3& units) {
		scaling *= XMLoad(&units);
		setNeedsUpdate();
	}

	void XM_CALLCONV scale(FXMVECTOR units) {
		scaling *= units;
		setNeedsUpdate();
	}

	void setScale(const f32_3& scale) {
		scaling = XMLoad(&scale);
		setNeedsUpdate();
	}

	void XM_CALLCONV setScale(FXMVECTOR scale) {
		scaling = scale;
		setNeedsUpdate();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Position, Rotation, and Scale (relative to parent, if any)
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
	// Member Functions - World Space Data
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisX() const {
		updateMatrix();
		return XMVector3Normalize(world.r[0]);
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisY() const {
		updateMatrix();
		return XMVector3Normalize(world.r[1]);
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisZ() const {
		updateMatrix();
		return XMVector3Normalize(world.r[2]);
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldOrigin() const {
		updateMatrix();
		return world.r[3];
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Matrices
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToWorldMatrix() const {
		updateMatrix();
		return world;
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getWorldToObjectMatrix() const {
		updateMatrix();
		return XMMatrixInverse(nullptr, world);
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentPositionMatrix() const {
		updateMatrix();
		return XMMatrixTranslationFromVector(translation);
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentRotationMatrix() const {
		updateMatrix();
		return XMMatrixRotationRollPitchYawFromVector(rotation);
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentScaleMatrix() const {
		updateMatrix();
		return XMMatrixScalingFromVector(scaling);
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The object-to-world matrix
	mutable XMMATRIX world = XMMatrixIdentity();

	// Translation, rotation, and scale vectors
	XMVECTOR translation = XMVectorZero();
	XMVECTOR rotation    = XMVectorZero();
	XMVECTOR scaling     = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

	// Determines if the transform has been modified, but not updated
	mutable bool needs_update = true;
};
