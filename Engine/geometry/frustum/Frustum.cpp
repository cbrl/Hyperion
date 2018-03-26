#include "stdafx.h"
#include "frustum.h"


Frustum::Frustum(CXMMATRIX in) {
	UpdateFrustum(in);
}


Frustum::~Frustum() {
}


void XM_CALLCONV Frustum::UpdateFrustum(FXMMATRIX in) {
	const XMMATRIX input = XMMatrixTranspose(in);

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
	for (u32 i = 0; i < 6; ++i) {
		planes[i] = XMPlaneNormalize(planes[i]);
	}
}



//----------------------------------------------------------------------------------
// Encloses - Object completely contained within frustum
//----------------------------------------------------------------------------------

bool XM_CALLCONV Frustum::Encloses(FXMVECTOR point) const {
	for (u32 i = 0; i < 6; ++i) {
		auto result = XMPlaneDotCoord(planes[i], point);
		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}


bool Frustum::Encloses(const AABB& aabb) const {

	// For each plane, get the minimum point of the AABB along the
	// plane's normal, then check which side of the plane the point is on.
	for (u32 i = 0; i < 6; ++i) {
		auto control = XMVectorGreaterOrEqual(planes[i], XMVectorZero());
		auto point   = XMVectorSelect(aabb.Max(), aabb.Min(), control);

		auto result = XMPlaneDotCoord(planes[i], point);

		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}


bool Frustum::Encloses(const BoundingSphere& sphere) const {

	// Calculate the dot product of the plane and the sphere's
	// center, then check if it's less than the sphere's radius
	for (u32 i = 0; i < 6; ++i) {
		auto result = XMPlaneDotCoord(planes[i], sphere.Center());

		if (XMVectorGetX(result) < sphere.Radius()) {
			return false;
		}
	}

	return true;
}




//----------------------------------------------------------------------------------
// Contains - Object completely or partially contained within frustum
//----------------------------------------------------------------------------------

bool Frustum::Contains(const AABB& aabb) const {
	
	// For each plane, get the minimum point of the AABB along the
	// plane's normal, then check which side of the plane the point is on.
	for (u32 i = 0; i < 6; ++i) {
		auto control = XMVectorGreaterOrEqual(planes[i], XMVectorZero());
		auto point   = XMVectorSelect(aabb.Min(), aabb.Max(), control);

		auto result = XMPlaneDotCoord(planes[i], point);

		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}


bool Frustum::Contains(const BoundingSphere& sphere) const {

	// Calculate the dot product of the plane and the sphere's
	// center, then check if it's less than the sphere's negative radius
	for (u32 i = 0; i < 6; ++i) {
		auto result = XMPlaneDotCoord(planes[i], sphere.Center());

		if (XMVectorGetX(result) < -sphere.Radius()) {
			return false;
		}
	}

	return true;
}