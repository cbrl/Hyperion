module;

#include <DirectXMath.h>

export module rendering:components.transform;

import ecs;
export import math.geometry;

using namespace DirectX;

namespace render::systems { class TransformSystem; }

export class Transform final : public ecs::Component, public Transform3D {
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
	Transform& operator=(Transform&& transform) noexcept = default;

protected:

	using Transform3D::needs_update;

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
