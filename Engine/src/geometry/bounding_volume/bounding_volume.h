#pragma once

#include "math/math.h"
#include "datatypes/datatypes.h"


struct AABB final {
public:
	AABB();

	// Construct an AABB from the min and max points on an object
	AABB(const float3& min, const float3& max);

	// Construct an AABB from the min and max points on an object
	AABB(FXMVECTOR min, FXMVECTOR max);

	~AABB() = default;


	// Transform the AABB by a matrix
	void XM_CALLCONV Transform(FXMMATRIX M);


	XMVECTOR XM_CALLCONV Min() const { return min_point; }
	XMVECTOR XM_CALLCONV Max() const { return max_point; }


private:
	XMVECTOR min_point;
	XMVECTOR max_point;

	// The non-transformed vertices of the AABB
	vector<XMVECTOR> origin_verts;
	// The transformed vertices of the AABB
	vector<XMVECTOR> transformed_verts;
};


struct BoundingSphere final {
public:
	BoundingSphere();

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(const float3& center, float radius);

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(FXMVECTOR center, float radius);

	~BoundingSphere() = default;


	// Transform the bounding sphere by a matrix
	void XM_CALLCONV Transform(FXMMATRIX M);


	XMVECTOR XM_CALLCONV Center() const { return transformed_center; }
	float Radius() const { return transformed_radius; }


private:
	// The non-transformed center and radius
	XMVECTOR origin_center;
	float origin_radius;

	// The transformed center and radius
	float transformed_radius;
	XMVECTOR transformed_center;
};
