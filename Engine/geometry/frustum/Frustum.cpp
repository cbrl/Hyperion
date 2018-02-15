#include "stdafx.h"
#include "Frustum.h"


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
	for (int i = 0; i < 6; ++i) {
		planes[i] = XMPlaneNormalize(planes[i]);
	}
}


/*bool Frustum::ContainsPoint(XMFLOAT3 point) {
	for (int i = 0; i < 6; ++i) {
		auto result = XMPlaneDotCoord(planes[i], XMLoadFloat3(&point));

		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}


bool Frustum::ContainsSphere(XMFLOAT3 center, float radius) {
	for (int i = 0; i < 6; ++i) {
		auto result = XMPlaneDotCoord(planes[i], XMLoadFloat3(&center));

		if (XMVectorGetX(result) < -radius) {
			return false;
		}
	}

	return true;
}


bool Frustum::ContainsCube(XMFLOAT3 center, float radius) {
	XMVECTOR v[8];


	v[0] = XMLoadFloat3(&XMFLOAT3((center.x - radius), (center.y - radius), (center.z - radius)));
	v[1] = XMLoadFloat3(&XMFLOAT3((center.x - radius), (center.y - radius), (center.z + radius)));
	v[2] = XMLoadFloat3(&XMFLOAT3((center.x - radius), (center.y + radius), (center.z - radius)));
	v[3] = XMLoadFloat3(&XMFLOAT3((center.x - radius), (center.y + radius), (center.z + radius)));
	v[4] = XMLoadFloat3(&XMFLOAT3((center.x + radius), (center.y - radius), (center.z - radius)));
	v[5] = XMLoadFloat3(&XMFLOAT3((center.x + radius), (center.y - radius), (center.z + radius)));
	v[6] = XMLoadFloat3(&XMFLOAT3((center.x + radius), (center.y + radius), (center.z - radius)));
	v[7] = XMLoadFloat3(&XMFLOAT3((center.x + radius), (center.y + radius), (center.z + radius)));


	for (int i = 0; i < 6; ++i) {
		bool cont = false;

		for (int j = 0; j < 8; ++j) {
			auto result = XMPlaneDotCoord(planes[i], v[j]);

			if (XMVectorGetX(result) >= 0.0f) {
				cont = true;
				break;
			}
		}

		if (cont) continue;
		else return false;
	}

	return true;
}


bool Frustum::ContainsBox(XMFLOAT3 center, XMFLOAT3 xyzSize) {
	XMVECTOR v[8];

	v[0] = XMLoadFloat3(&XMFLOAT3((center.x - xyzSize.x), (center.y - xyzSize.y), (center.z - xyzSize.z)));
	v[1] = XMLoadFloat3(&XMFLOAT3((center.x - xyzSize.x), (center.y - xyzSize.y), (center.z + xyzSize.z)));
	v[2] = XMLoadFloat3(&XMFLOAT3((center.x - xyzSize.x), (center.y + xyzSize.y), (center.z - xyzSize.z)));
	v[3] = XMLoadFloat3(&XMFLOAT3((center.x - xyzSize.x), (center.y + xyzSize.y), (center.z + xyzSize.z)));
	v[4] = XMLoadFloat3(&XMFLOAT3((center.x + xyzSize.x), (center.y - xyzSize.y), (center.z - xyzSize.z)));
	v[5] = XMLoadFloat3(&XMFLOAT3((center.x + xyzSize.x), (center.y - xyzSize.y), (center.z + xyzSize.z)));
	v[6] = XMLoadFloat3(&XMFLOAT3((center.x + xyzSize.x), (center.y + xyzSize.y), (center.z - xyzSize.z)));
	v[7] = XMLoadFloat3(&XMFLOAT3((center.x + xyzSize.x), (center.y + xyzSize.y), (center.z + xyzSize.z)));


	for (int i = 0; i < 6; ++i) {
		bool cont = false;

		for (int j = 0; j < 8; ++j) {
			auto result = XMPlaneDotCoord(planes[i], v[j]);

			if (XMVectorGetX(result) >= 0.0f) {
				cont = true;
				break;
			}
		}

		if (cont) continue;
		else return false;
	}

	return true;
}*/


//----------------------------------------------------------------------------------
// Encloses - Object completely contained within frustum
//----------------------------------------------------------------------------------

bool Frustum::Encloses(const XMVECTOR& point) const {
	for (int i = 0; i < 6; ++i) {
		auto result = XMPlaneDotCoord(planes[i], point);
		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}


bool Frustum::Encloses(const AABB& aabb) const {
	// Get the minimum point on the plane's normal and check what side it's on
	for (int i = 0; i < 6; ++i) {
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
	for (int i = 0; i < 6; ++i) {
		auto control = XMVectorGreaterOrEqual(planes[i], XMVectorZero());
		auto point = XMVectorSelect(aabb.Min(), aabb.Max(), control);
		auto result = XMPlaneDotCoord(planes[i], point);

		if (XMVectorGetX(result) < 0.0f) {
			return false;
		}
	}

	return true;
}