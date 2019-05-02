#pragma once

#include "datatypes/datatypes.h"
#include "directx/math/directx_math.h"


namespace render {

struct AABB final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	constexpr AABB() noexcept
	    : min_point({std::numeric_limits<f32>::infinity()})
	    , max_point({-std::numeric_limits<f32>::infinity()}) {
	}

	// Construct an AABB from the min and max points of an object
	AABB(const f32_3& min, const f32_3& max) noexcept
		: min_point(XMLoad(&min))
		, max_point(XMLoad(&max)) {
	}

	// Construct an AABB from the min and max points of an object
	constexpr AABB(FXMVECTOR min, FXMVECTOR max) noexcept
	    : min_point(min)
	    , max_point(max) {
	}

	constexpr AABB(const AABB& aabb) noexcept = default;
	constexpr AABB(AABB&& aabb) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~AABB() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr AABB& operator=(const AABB& aabb) noexcept = default;
	constexpr AABB& operator=(AABB&& aabb) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV min() const noexcept { return min_point; }

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV max() const noexcept { return max_point; }


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

	constexpr BoundingSphere() noexcept
	    : sphere_center({0.0f})
	    , sphere_radius(std::numeric_limits<f32>::infinity()) {
	}

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(const f32_3& center, f32 radius) noexcept
	    : sphere_center(XMLoad(&center))
	    , sphere_radius(radius) {
	}

	// Construct a bounding sphere from the center point and radius of an object
	constexpr BoundingSphere(FXMVECTOR center, f32 radius) noexcept
	    : sphere_center(center)
	    , sphere_radius(radius) {
	}

	constexpr BoundingSphere(const BoundingSphere& sphere) noexcept = default;
	constexpr BoundingSphere(BoundingSphere&& sphere) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~BoundingSphere() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr BoundingSphere& operator=(const BoundingSphere& sphere) noexcept = default;
	constexpr BoundingSphere& operator=(BoundingSphere&& sphere) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV center() const noexcept { return sphere_center; }

	[[nodiscard]]
	constexpr f32 radius() const noexcept { return sphere_radius; }


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	XMVECTOR sphere_center;
	f32      sphere_radius;
};

} //namespace render
