#pragma once

#include "directx/directx_math.h"
#include "datatypes/datatypes.h"
#include "component/component.h"


#include "event/event.h"
class Transform;
struct TransformEvent : public Event<TransformEvent> {
	TransformEvent(Transform& transform) : transform(transform) {}
	std::reference_wrapper<Transform> transform;
};


class Transform final : public Component<Transform>, public EventSender {
	friend class TransformSystem;

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

	void sendUpdateEvent() {
		needs_update = true;
		this->sendEvent<TransformEvent>(std::ref(*this));
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Position
	//----------------------------------------------------------------------------------

	void moveX(f32 units) {
		translation += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		sendUpdateEvent();
	}

	void moveY(f32 units) {
		translation += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		sendUpdateEvent();
	}

	void moveZ(f32 units) {
		translation += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		sendUpdateEvent();
	}

	void move(const vec3_f32& units) {
		translation += XMLoad(&units);
		sendUpdateEvent();
	}

	void XM_CALLCONV move(FXMVECTOR units) {
		translation += units;
		sendUpdateEvent();
	}

	void setPosition(const vec3_f32& position) {
		translation = XMLoad(&position);
		sendUpdateEvent();
	}

	void XM_CALLCONV setPosition(FXMVECTOR position) {
		translation = position;
		sendUpdateEvent();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Rotation
	//----------------------------------------------------------------------------------

	void rotateX(f32 units) {
		rotation += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		sendUpdateEvent();
	}

	void rotateY(f32 units) {
		rotation += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		sendUpdateEvent();
	}

	void rotateZ(f32 units) {
		rotation += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		sendUpdateEvent();
	}

	void rotateXClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetX(rotation) + units, min, max);
		rotation = XMVectorSetX(rotation, amount);
		sendUpdateEvent();
	}

	void rotateYClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetY(rotation) + units, min, max);
		rotation = XMVectorSetY(rotation, amount);
		sendUpdateEvent();
	}

	void rotateZClamped(f32 units, f32 min, f32 max) {
		const f32 amount = ClampAngle(XMVectorGetZ(rotation) + units, min, max);
		rotation = XMVectorSetZ(rotation, amount);
		sendUpdateEvent();
	}

	void rotate(const vec3_f32& units) {
		rotation += XMLoad(&units);
		sendUpdateEvent();
	}

	void XM_CALLCONV rotate(FXMVECTOR units) {
		rotation += units;
		sendUpdateEvent();
	}

	void rotateClamped(const vec3_f32& units, f32 min, f32 max) {
		rotateXClamped(units.x, min, max);
		rotateYClamped(units.y, min, max);
		rotateZClamped(units.z, min, max);
	}

	void XM_CALLCONV rotateClamped(FXMVECTOR units, f32 min, f32 max) {
		rotateXClamped(XMVectorGetX(units), min, max);
		rotateYClamped(XMVectorGetY(units), min, max);
		rotateZClamped(XMVectorGetZ(units), min, max);
	}

	void rotateAround(vec3_f32 axis, f32 units) {
		rotateAround(XMLoad(&axis), units);
	}

	void XM_CALLCONV rotateAround(FXMVECTOR axis, f32 units) {
		translation = XMVector3Transform(translation, XMMatrixRotationAxis(axis, units));
		sendUpdateEvent();
	}

	void setRotation(const vec3_f32& rotation) {
		this->rotation = XMLoad(&rotation);
		sendUpdateEvent();
	}

	void XM_CALLCONV setRotation(FXMVECTOR rotation) {
		this->rotation = rotation;
		sendUpdateEvent();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Scale
	//----------------------------------------------------------------------------------

	void scaleX(f32 units) {
		scaling += XMVectorSet(units, 0.0f, 0.0f, 0.0f);
		sendUpdateEvent();
	}

	void scaleY(f32 units) {
		scaling += XMVectorSet(0.0f, units, 0.0f, 0.0f);
		sendUpdateEvent();
	}

	void scaleZ(f32 units) {
		scaling += XMVectorSet(0.0f, 0.0f, units, 0.0f);
		sendUpdateEvent();
	}

	void scale(const vec3_f32& units) {
		scaling *= XMLoad(&units);
		sendUpdateEvent();
	}

	void XM_CALLCONV scale(FXMVECTOR units) {
		scaling *= units;
		sendUpdateEvent();
	}

	void setScale(const vec3_f32& scale) {
		scaling = XMLoad(&scale);
		sendUpdateEvent();
	}

	void XM_CALLCONV setScale(FXMVECTOR scale) {
		scaling = scale;
		sendUpdateEvent();
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
