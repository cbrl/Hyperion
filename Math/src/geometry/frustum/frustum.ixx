module;

#include "directxmath/directxmath.h"

export module math.geometry.frustum;

export import math.geometry.bounding_volume;


export class Frustum final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Frustum(CXMMATRIX M){
		const XMMATRIX input = XMMatrixTranspose(M);

		// Near plane
		planes[0] = input.r[2];

		// Far plane
		planes[1] = input.r[3] - input.r[2];

		// Left plane
		planes[2] = input.r[3] + input.r[0];

		// Right plane
		planes[3] = input.r[3] - input.r[0];

		// Top plane
		planes[4] = input.r[3] - input.r[1];

		// Bottom plane
		planes[5] = input.r[3] + input.r[1];

		// Normalize the planes
		for (auto& plane : planes) {
			plane = XMPlaneNormalize(plane);
		}
	}

	Frustum(const Frustum& frustum) noexcept = default;
	Frustum(Frustum&& frustum) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Frustum() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Frustum& operator=(const Frustum& frustum) noexcept = default;
	Frustum& operator=(Frustum&& frustum) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Encloses - Object completely contained within frustum
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool XM_CALLCONV encloses(FXMVECTOR point) const {
		for (auto plane : planes) {
			const auto result = XMPlaneDotCoord(plane, point);
			if (XMVectorGetX(result) < 0.0f) {
				return false;
			}
		}

		return true;
	}

	[[nodiscard]]
	bool encloses(const AABB& aabb) const {
		// For each plane, get the minimum point of the AABB along the
		// plane's normal, then check which side of the plane the point is on.
		for (auto plane : planes) {
			const auto control = XMVectorGreaterOrEqual(plane, XMVectorZero());
			const auto point   = XMVectorSelect(aabb.max(), aabb.min(), control);

			const auto result = XMPlaneDotCoord(plane, point);

			if (XMVectorGetX(result) < 0.0f) {
				return false;
			}
		}

		return true;
	}

	[[nodiscard]]
	bool encloses(const BoundingSphere& sphere) const {
		// Calculate the dot product of the plane and the sphere's
		// center, then check if it's less than the sphere's radius
		for (auto plane : planes) {
			const auto result = XMPlaneDotCoord(plane, sphere.center());

			if (XMVectorGetX(result) < sphere.radius()) {
				return false;
			}
		}

		return true;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Contains - Object completely or partially contained in frustum
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(const AABB& aabb) const{
		// For each plane, get the minimum point of the AABB along the
		// plane's normal, then check which side of the plane the point is on.
		for (auto plane : planes) {
			const auto control = XMVectorGreaterOrEqual(plane, XMVectorZero());
			const auto point   = XMVectorSelect(aabb.min(), aabb.max(), control);

			const auto result = XMPlaneDotCoord(plane, point);

			if (XMVectorGetX(result) < 0.0f) {
				return false;
			}
		}

		return true;
	}

	[[nodiscard]]
	bool contains(const BoundingSphere& sphere) const {
		// Calculate the dot product of the plane and the sphere's
		// center, then check if it's less than the sphere's negative radius
		for (auto plane : planes) {
			const auto result = XMPlaneDotCoord(plane, sphere.center());

			if (XMVectorGetX(result) < -sphere.radius()) {
				return false;
			}
		}

		return true;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	XMVECTOR planes[6] = {};
};
