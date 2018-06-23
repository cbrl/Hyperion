#pragma once

#include "datatypes/datatypes.h"
#include "directx/directx_math.h"


struct AABB final {
public:
	AABB();
	AABB(const AABB& aabb) = default;
	AABB(AABB&& aabb) = default;

	// Construct an AABB from the min and max points of an object
	AABB(const f32_3& min, const f32_3& max);

	// Construct an AABB from the min and max points of an object
	AABB(FXMVECTOR min, FXMVECTOR max);

	~AABB() = default;


	AABB& operator=(const AABB& aabb) = default;
	AABB& operator=(AABB&& aabb) = default;


	[[nodiscard]]
	XMVECTOR XM_CALLCONV min() const { return min_point; }

	[[nodiscard]]
	XMVECTOR XM_CALLCONV max() const { return max_point; }


private:
	XMVECTOR min_point;
	XMVECTOR max_point;
};


struct BoundingSphere final {
public:
	BoundingSphere();
	BoundingSphere(const BoundingSphere& sphere) = default;
	BoundingSphere(BoundingSphere&& sphere) = default;

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(const f32_3& center, float radius);

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(FXMVECTOR center, float radius);

	~BoundingSphere() = default;


	BoundingSphere& operator=(const BoundingSphere& sphere) = default;
	BoundingSphere& operator=(BoundingSphere&& sphere) = default;


	[[nodiscard]]
	XMVECTOR XM_CALLCONV center() const { return sphere_center; }

	[[nodiscard]]
	float radius() const { return sphere_radius; }


private:
	// The non-transformed center and radius
	XMVECTOR sphere_center;
	float sphere_radius;
};
