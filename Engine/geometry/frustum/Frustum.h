#pragma once

#include "util\datatypes\datatypes.h"
#include "geometry\bounding_volume\bounding_volume.h"

using namespace DirectX;

class Frustum final {
	public:
		Frustum() = default;

		// Construct a frustum from a given matrix
		Frustum(CXMMATRIX M);

		~Frustum() = default;

		// Recalculate the frustum
		void XM_CALLCONV UpdateFrustum(FXMMATRIX M);


		bool XM_CALLCONV Encloses(FXMVECTOR point) const;

		bool Encloses(const AABB& aabb) const;
		bool Contains(const AABB& aabb) const;

		bool Encloses(const BoundingSphere& sphere) const;
		bool Contains(const BoundingSphere& sphere) const;


	private:
		XMVECTOR planes[6];
};