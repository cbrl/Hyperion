#include "stdafx.h"
#include "Frustum.h"


Frustum::Frustum(XMMATRIX viewProj) {
	viewProj = XMMatrixTranspose(viewProj);

	// Near plane
	planes[0] = viewProj.r[2];

	// Far plane
	planes[1] = viewProj.r[3] - viewProj.r[2];

	// Left plane
	planes[2] = viewProj.r[3] + viewProj.r[0];

	// Right plane
	planes[3] = viewProj.r[3] - viewProj.r[0];

	// Top plane
	planes[4] = viewProj.r[3] - viewProj.r[1];

	// Bottom plane
	planes[5] = viewProj.r[3] + viewProj.r[1];
}


Frustum::~Frustum() {
}


bool Frustum::ContainsPoint(XMFLOAT3 point) {
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

		for (int j = 0; j < 6; ++j) {
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

		for (int j = 0; j < 6; ++j) {
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
