#pragma once

#include "util\datatypes\datatypes.h"
#include "geometry\boundingvolume\bounding_volume.h"

using namespace DirectX;

class Frustum final {
	public:
		Frustum() = default;
		Frustum(CXMMATRIX viewProj);
		~Frustum();

		void XM_CALLCONV UpdateFrustum(FXMMATRIX viewProj);

		bool XM_CALLCONV Encloses(FXMVECTOR point) const;

		bool Encloses(const AABB& aabb) const;
		bool Contains(const AABB& aabb) const;

		bool Encloses(const BoundingSphere& sphere) const;
		bool Contains(const BoundingSphere& sphere) const;


	private:
		XMVECTOR planes[6];
};