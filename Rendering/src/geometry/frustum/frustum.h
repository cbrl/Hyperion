#pragma once

#include "datatypes/datatypes.h"
#include "geometry/bounding_volume/bounding_volume.h"


namespace render {

class Frustum final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Frustum(CXMMATRIX M);
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
	// Member Functions - Encloses
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool XM_CALLCONV encloses(FXMVECTOR point) const;

	[[nodiscard]]
	bool encloses(const AABB& aabb) const;

	[[nodiscard]]
	bool encloses(const BoundingSphere& sphere) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Contains
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(const AABB& aabb) const;

	[[nodiscard]]
	bool contains(const BoundingSphere& sphere) const;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	XMVECTOR planes[6] = {};
};

} //namespace render
