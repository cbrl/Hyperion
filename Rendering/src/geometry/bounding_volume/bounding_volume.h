#pragma once

#include "datatypes/datatypes.h"
#include "directx/directx_math.h"


struct AABB final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	AABB();

	// Construct an AABB from the min and max points of an object
	AABB(const vec3_f32& min, const vec3_f32& max);

	// Construct an AABB from the min and max points of an object
	AABB(FXMVECTOR min, FXMVECTOR max);

	AABB(const AABB& aabb) = default;
	AABB(AABB&& aabb) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~AABB() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	AABB& operator=(const AABB& aabb) = default;
	AABB& operator=(AABB&& aabb) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMVECTOR XM_CALLCONV min() const { return min_point; }

	[[nodiscard]]
	XMVECTOR XM_CALLCONV max() const { return max_point; }


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	XMVECTOR min_point;
	XMVECTOR max_point;
};


struct BoundingSphere final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	BoundingSphere();

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(const vec3_f32& center, f32 radius);

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(FXMVECTOR center, f32 radius);

	BoundingSphere(const BoundingSphere& sphere) = default;
	BoundingSphere(BoundingSphere&& sphere) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~BoundingSphere() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	BoundingSphere& operator=(const BoundingSphere& sphere) = default;
	BoundingSphere& operator=(BoundingSphere&& sphere) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMVECTOR XM_CALLCONV center() const { return sphere_center; }

	[[nodiscard]]
	f32 radius() const { return sphere_radius; }


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	XMVECTOR sphere_center;
	f32 sphere_radius;
};
