#pragma once

#include "component/component.h"
#include "datatypes/datatypes.h"
#include "directx/math/directx_math.h"
#include "scene/events/core_events.h"


namespace render {

namespace systems { class TransformSystem; }

class Transform final : public ecs::Component<Transform>, public ecs::EventSender {
	friend class systems::TransformSystem;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Transform() noexcept
		: world(XMMatrixIdentity())
		, translation(XMVectorZero())
		, rotation(XMVectorZero())
		, scaling(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f))
		, needs_update(true) {
	}

	Transform(const Transform& transform) = delete;
	Transform(Transform&& transform) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Transform() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Transform& operator=(const Transform& transform) = delete;
	Transform& operator=(Transform&& transform) = default;



	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------

	void sendNeedsUpdateEvent() {
		needs_update = true;
		this->sendEvent<events::TransformNeedsUpdate>(std::ref(*this));
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Position
	//----------------------------------------------------------------------------------

	void moveX(f32 units) {
		translation += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		sendNeedsUpdateEvent();
	}

	void moveY(f32 units) {
		translation += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		sendNeedsUpdateEvent();
	}

	void moveZ(f32 units) {
		translation += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		sendNeedsUpdateEvent();
	}

	void move(const f32_3& units) {
		translation += XMLoad(&units);
		sendNeedsUpdateEvent();
	}

	void XM_CALLCONV move(FXMVECTOR units) {
		translation += units;
		sendNeedsUpdateEvent();
	}

	void setPosition(const f32_3& position) {
		translation = XMLoad(&position);
		sendNeedsUpdateEvent();
	}

	void XM_CALLCONV setPosition(FXMVECTOR position) {
		translation = position;
		sendNeedsUpdateEvent();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Rotation
	//----------------------------------------------------------------------------------

	void rotateX(f32 units) {
		rotation += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		sendNeedsUpdateEvent();
	}

	void rotateY(f32 units) {
		rotation += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		sendNeedsUpdateEvent();
	}

	void rotateZ(f32 units) {
		rotation += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		sendNeedsUpdateEvent();
	}

	void rotateXClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetX(rotation) + units, min, max);
		rotation = XMVectorSetX(rotation, amount);
		sendNeedsUpdateEvent();
	}

	void rotateYClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetY(rotation) + units, min, max);
		rotation = XMVectorSetY(rotation, amount);
		sendNeedsUpdateEvent();
	}

	void rotateZClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetZ(rotation) + units, min, max);
		rotation = XMVectorSetZ(rotation, amount);
		sendNeedsUpdateEvent();
	}

	void rotate(const f32_3& units) {
		rotation += XMLoad(&units);
		sendNeedsUpdateEvent();
	}

	void XM_CALLCONV rotate(FXMVECTOR units) {
		rotation += units;
		sendNeedsUpdateEvent();
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
	}

	void XM_CALLCONV rotateAround(FXMVECTOR axis, f32 units) {
		translation = XMVector3Transform(translation, XMMatrixRotationAxis(axis, units));
		sendNeedsUpdateEvent();
	}

	void setRotation(const f32_3& rotation) {
		this->rotation = XMLoad(&rotation);
		sendNeedsUpdateEvent();
	}

	void XM_CALLCONV setRotation(FXMVECTOR rotation) {
		this->rotation = rotation;
		sendNeedsUpdateEvent();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Scale
	//----------------------------------------------------------------------------------

	void scaleX(f32 units) {
		scaling += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		sendNeedsUpdateEvent();
	}

	void scaleY(f32 units) {
		scaling += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		sendNeedsUpdateEvent();
	}

	void scaleZ(f32 units) {
		scaling += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		sendNeedsUpdateEvent();
	}

	void scale(const f32_3& units) {
		scaling *= XMLoad(&units);
		sendNeedsUpdateEvent();
	}

	void XM_CALLCONV scale(FXMVECTOR units) {
		scaling *= units;
		sendNeedsUpdateEvent();
	}

	void setScale(const f32_3& scale) {
		scaling = XMLoad(&scale);
		sendNeedsUpdateEvent();
	}

	void XM_CALLCONV setScale(FXMVECTOR scale) {
		scaling = scale;
		sendNeedsUpdateEvent();
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
	// Member Functions - Object Space Data
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	static XMVECTOR XM_CALLCONV getObjectAxisX() {
		static auto out = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		return out;
	}

	[[nodiscard]]
	static XMVECTOR XM_CALLCONV getObjectAxisY() {
		static auto out = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		return out;
	}

	[[nodiscard]]
	static XMVECTOR XM_CALLCONV getObjectAxisZ() {
		static auto out = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		return out;
	}

	[[nodiscard]]
	static XMVECTOR XM_CALLCONV getObjectOrigin() {
		static auto out = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		return out;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Matrices
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
	// The object-to-world matrix
	XMMATRIX world;

	// Translation, rotation, and scale vectors
	XMVECTOR translation;
	XMVECTOR rotation;
	XMVECTOR scaling;

	// Determines if the transform has been modified, but not updated
	bool needs_update;
};

} //namespace render