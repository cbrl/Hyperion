module;

#include <DirectXMath.h>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

export module math.geometry:transform_3d;

import math.directxmath;

using namespace DirectX;

export class Transform3D {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Transform3D() {
		world = XMMatrixIdentity();
		translation = XMVectorZero();
		rotation = XMVectorZero();
		scaling = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	}

	Transform3D(const Transform3D& transform) = default;
	Transform3D(Transform3D&& transform) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~Transform3D() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Transform3D& operator=(const Transform3D& transform) = default;
	Transform3D& operator=(Transform3D&& transform) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------

	bool needsUpdate() const noexcept {
		return needs_update;
	}

	void setNeedsUpdate() {
		needs_update = true;
	}

	virtual void updateMatrix() const {
		if (needs_update) {
			world = XMMatrixScalingFromVector(getRelativeScale())
			        * XMMatrixRotationRollPitchYawFromVector(getRelativeRotation())
			        * XMMatrixTranslationFromVector(getRelativePosition());
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
		rotateXClamped(units, -XM_PI, XM_PI);
	}

	void rotateY(f32 units) {
		rotateYClamped(units, -XM_PI, XM_PI);
	}

	void rotateZ(f32 units) {
		rotateZClamped(units, -XM_PI, XM_PI);
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
		rotateClamped(units, -XM_PI, XM_PI);
	}

	void XM_CALLCONV rotate(FXMVECTOR units) {
		rotateClamped(units, -XM_PI, XM_PI);
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
	XMVECTOR XM_CALLCONV getRelativePosition() const {
		return translation;
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getRelativeRotation() const {
		return rotation;
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getRelativeScale() const {
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

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldScale() const {
		updateMatrix();
		return XMVectorSet(
			XMVectorGetX(XMVector3Length(world.r[0])),
			XMVectorGetX(XMVector3Length(world.r[1])),
			XMVectorGetX(XMVector3Length(world.r[2])),
			0
		);
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

protected:

	void clearNeedsUpdate() const noexcept {
		needs_update = false;
	}

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The object-to-world matrix
	mutable XMMATRIX world;

	// Translation, rotation, and scale vectors
	XMVECTOR translation;
	XMVECTOR rotation;   
	XMVECTOR scaling;

	// Determines if the transform has been modified, but not updated
	mutable bool needs_update = true;
};
