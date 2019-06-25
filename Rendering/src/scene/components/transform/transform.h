#pragma once

#include "component/component.h"
#include "entity/entity.h"
#include "geometry/transform/transform_3d.h"
#include "directxmath/directxmath.h"
#include "scene/events/core_events.h"


namespace render { namespace systems { class TransformSystem; } }

class Transform final : public ecs::Component<Transform>, public Transform3D {
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

private:

	// Don't want the matrix to self-update as a component. It can cause the transform
	// hierarchy to not be updated correctly.
	void updateMatrix() const override {
		return;
	}

	// Called by TransformSystem
	void update(const XMMATRIX* parent = nullptr) const {
		if (needs_update) {
			Transform3D::updateMatrix();
			if (parent)
				world *= *parent;
			needs_update = false;
		}
	}
};
