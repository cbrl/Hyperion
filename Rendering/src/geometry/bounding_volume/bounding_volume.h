#pragma once

#include "datatypes/datatypes.h"
#include "directx/math/directx_math.h"


namespace render {

struct AABB final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	AABB() noexcept = default;

	// Construct an AABB from the min and max points of an object
	AABB(const f32_3& min, const f32_3& max) noexcept
		: min_point(XMLoad(&min))
		, max_point(XMLoad(&max)) {
	}

	// Construct an AABB from the min and max points of an object
	AABB(FXMVECTOR min, FXMVECTOR max) noexcept
	    : min_point(min)
	    , max_point(max) {
	}

	AABB(const AABB& aabb) noexcept = default;
	AABB(AABB&& aabb) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~AABB() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	AABB& operator=(const AABB& aabb) noexcept = default;
	AABB& operator=(AABB&& aabb) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMVECTOR XM_CALLCONV min() const noexcept { return min_point; }

	[[nodiscard]]
	XMVECTOR XM_CALLCONV max() const noexcept { return max_point; }

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	XMVECTOR min_point = {std::numeric_limits<f32>::infinity()};
	XMVECTOR max_point = {-std::numeric_limits<f32>::infinity()};
};


struct BoundingSphere final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	BoundingSphere() noexcept = default;

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(const f32_3& center, f32 radius) noexcept
	    : sphere_center(XMLoad(&center))
	    , sphere_radius(radius) {
	}

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(FXMVECTOR center, f32 radius) noexcept
	    : sphere_center(center)
	    , sphere_radius(radius) {
	}

	BoundingSphere(const BoundingSphere& sphere) noexcept = default;
	BoundingSphere(BoundingSphere&& sphere) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~BoundingSphere() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	BoundingSphere& operator=(const BoundingSphere& sphere) noexcept = default;
	BoundingSphere& operator=(BoundingSphere&& sphere) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	XMVECTOR XM_CALLCONV center() const noexcept { return sphere_center; }

	[[nodiscard]]
	f32 radius() const noexcept { return sphere_radius; }

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	XMVECTOR sphere_center = {0.0f};
	f32      sphere_radius = std::numeric_limits<f32>::infinity();
};

} //namespace render
