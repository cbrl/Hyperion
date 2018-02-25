#include "stdafx.h"
#include "frustum.h"


Frustum::Frustum(const XMMATRIX& in) {
	CreateFrustum(in);
}


Frustum::~Frustum() {
}


void Frustum::CreateFrustum(const XMMATRIX& in) {
	XMMATRIX input = XMMatrixTranspose(in);

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
	for (i32 i = 0; i < 6; ++i) {
		planes[i] = XMPlaneNormalize(planes[i]);
	}
}



//----------------------------------------------------------------------------------
// Encloses - Object completely contained within frustum
//----------------------------------------------------------------------------------

bool Frustum::Encloses(const XMVECTOR& point) const {
	for (i32 i = 0; i < 6; ++i) {
		auto result = XMPlaneDotCoord(planes[i], point);
		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}


bool Frustum::Encloses(const AABB& aabb) const {
	// Get the minimum point on the plane's normal and check what side it's on
	for (i32 i = 0; i < 6; ++i) {
		auto control = XMVectorGreaterOrEqual(planes[i], XMVectorZero());
		auto point = XMVectorSelect(aabb.Max(), aabb.Min(), control);
		auto result = XMPlaneDotCoord(planes[i], point);

		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}




//----------------------------------------------------------------------------------
// Contains - Object completely or partially contained within frustum
//----------------------------------------------------------------------------------

bool Frustum::Contains(const AABB& aabb) const {
	// Get the maximin point on the plane's normal and check what side it's on
	for (i32 i = 0; i < 6; ++i) {
		auto control = XMVectorGreaterOrEqual(planes[i], XMVectorZero());
		auto point = XMVectorSelect(aabb.Min(), aabb.Max(), control);
		auto result = XMPlaneDotCoord(planes[i], point);

		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}