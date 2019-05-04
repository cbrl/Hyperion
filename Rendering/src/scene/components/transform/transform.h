#pragma once

#include "component/component.h"
#include "entity/entity.h"
#include "geometry/transform/transform_3d.h"
#include "directx/math/directx_math.h"
#include "scene/events/core_events.h"


namespace render { namespace systems { class TransformSystem; } }

class Transform final : public ecs::Component<Transform>, public ecs::EventSender {
	friend class render::systems::TransformSystem;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Transform() = default;
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
	// Member Functions - Position
	//----------------------------------------------------------------------------------

	void moveX(f32 units) {
		transform.moveX(units);
		setNeedsUpdate();
	}

	void moveY(f32 units) {
		transform.moveY(units);
		setNeedsUpdate();
	}

	void moveZ(f32 units) {
		transform.moveZ(units);
		setNeedsUpdate();
	}

	void move(const f32_3& units) {
		transform.move(units);
		setNeedsUpdate();
	}

	void XM_CALLCONV move(FXMVECTOR units) {
		transform.move(units);
		setNeedsUpdate();
	}

	void setPosition(const f32_3& position) {
		transform.setPosition(position);
		setNeedsUpdate();
	}

	void XM_CALLCONV setPosition(FXMVECTOR position) {
		transform.setPosition(position);
		setNeedsUpdate();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Rotation
	//----------------------------------------------------------------------------------

	void rotateX(f32 units) {
		transform.rotateX(units);
		setNeedsUpdate();
	}

	void rotateY(f32 units) {
		transform.rotateY(units);
		setNeedsUpdate();
	}

	void rotateZ(f32 units) {
		transform.rotateZ(units);
		setNeedsUpdate();
	}

	void rotateXClamped(f32 units, f32 min, f32 max) {
		transform.rotateXClamped(units, min, max);
		setNeedsUpdate();
	}

	void rotateYClamped(f32 units, f32 min, f32 max) {
		transform.rotateYClamped(units, min, max);
		setNeedsUpdate();
	}

	void rotateZClamped(f32 units, f32 min, f32 max) {
		transform.rotateZClamped(units, min, max);
		setNeedsUpdate();
	}

	void rotate(const f32_3& units) {
		transform.rotate(units);
		setNeedsUpdate();
	}

	void XM_CALLCONV rotate(FXMVECTOR units) {
		transform.rotate(units);
		setNeedsUpdate();
	}

	void rotateClamped(const f32_3& units, f32 min, f32 max) {
		transform.rotateClamped(units, min, max);
		setNeedsUpdate();
	}

	void XM_CALLCONV rotateClamped(FXMVECTOR units, f32 min, f32 max) {
		transform.rotateClamped(units, min, max);
		setNeedsUpdate();
	}

	void rotateAround(f32_3 axis, f32 units) {
		transform.rotateAround(axis, units);
		setNeedsUpdate();
	}

	void XM_CALLCONV rotateAround(FXMVECTOR axis, f32 units) {
		transform.rotateAround(axis, units);
		setNeedsUpdate();
	}

	void setRotation(const f32_3& rotation) {
		transform.setRotation(rotation);
		setNeedsUpdate();
	}

	void XM_CALLCONV setRotation(FXMVECTOR rotation) {
		transform.setRotation(rotation);
		setNeedsUpdate();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Scale
	//----------------------------------------------------------------------------------

	void scaleX(f32 units) {
		transform.scaleX(units);
		setNeedsUpdate();
	}

	void scaleY(f32 units) {
		transform.scaleY(units);
		setNeedsUpdate();
	}

	void scaleZ(f32 units) {
		transform.scaleZ(units);
		setNeedsUpdate();
	}

	void scale(const f32_3& units) {
		transform.scale(units);
		setNeedsUpdate();
	}

	void XM_CALLCONV scale(FXMVECTOR units) {
		transform.scale(units);
		setNeedsUpdate();
	}

	void setScale(const f32_3& scale) {
		transform.setScale(scale);
		setNeedsUpdate();
	}

	void XM_CALLCONV setScale(FXMVECTOR scale) {
		transform.setScale(scale);
		setNeedsUpdate();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Position, Rotation, and Scale (relative to parent, if any)
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getPosition() const {
		return transform.getPosition();
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getRotation() const {
		return transform.getRotation();
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getScale() const {
		return transform.getScale();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - World Space Data
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisX() const {
		update();
		return transform.getWorldAxisX();
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisY() const {
		update();
		return transform.getWorldAxisY();
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldAxisZ() const {
		update();
		return transform.getWorldAxisZ();
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getWorldOrigin() const {
		update();
		return transform.getWorldOrigin();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Matrices
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToWorldMatrix() const {
		update();
		return transform.getObjectToWorldMatrix();
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getWorldToObjectMatrix() const {
		update();
		return transform.getWorldToObjectMatrix();
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentPositionMatrix() const {
		update();
		return transform.getObjectToParentPositionMatrix();
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentRotationMatrix() const {
		update();
		return transform.getObjectToParentRotationMatrix();
	}

	[[nodiscard]]
	XMMATRIX XM_CALLCONV getObjectToParentScaleMatrix() const {
		update();
		return transform.getObjectToParentScaleMatrix();
	}

private:

	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------
	void setNeedsUpdate() const {
		needs_update = true;
	}

	void update() const {
		if (not needs_update)
			return;

		if (getOwner()->hasParent()) { //update parent transform if there is one
			const auto* parent_transform = getOwner()->getParent()->getComponent<Transform>();
			if (parent_transform) {
				const XMMATRIX parent_matrix = parent_transform->getObjectToWorldMatrix(); //calls parent->update()
				transform.updateMatrix(&parent_matrix);
			}
		}
		else {
			transform.updateMatrix();
		}

		needs_update = false;

		// Update all child transforms (if any)
		getOwner()->forEachChild([](ecs::EntityPtr& child) {
			if (auto* transform = child->getComponent<Transform>()) {
				transform->transform.setNeedsUpdate(); // Manually set 'needs update' flag of child
				transform->setNeedsUpdate();
				transform->update();
			}
		});
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	Transform3D transform;
	
	// Determines if the transform has been modified, but not updated.
	mutable bool needs_update = true;
};