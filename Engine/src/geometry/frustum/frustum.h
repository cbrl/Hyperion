#pragma once

#include "datatypes/datatypes.h"
#include "geometry/bounding_volume/bounding_volume.h"

using namespace DirectX;


class Frustum final {
public:
	Frustum() = default;

	// Construct a frustum from a given matrix
	Frustum(CXMMATRIX M);

	~Frustum() = default;

	// Recalculate the frustum
	void XM_CALLCONV updateFrustum(FXMMATRIX M);


	bool XM_CALLCONV encloses(FXMVECTOR point) const;

	bool encloses(const AABB& aabb) const;
	bool contains(const AABB& aabb) const;

	bool encloses(const BoundingSphere& sphere) const;
	bool contains(const BoundingSphere& sphere) const;


private:
	XMVECTOR planes[6];
};
