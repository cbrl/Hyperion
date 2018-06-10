#include "bounding_volume.h"


//----------------------------------------------------------------------------------
// AABB
//----------------------------------------------------------------------------------
AABB::AABB()
	: min_point(g_XMInfinity)
	, max_point(g_XMNegInfinity)
	, origin_verts(8)
	, transformed_verts(8) {
}


AABB::AABB(const float3& min, const float3& max)
	: min_point(XMLoadFloat3(&min))
	, max_point(XMLoadFloat3(&max))
	, origin_verts(8)
	, transformed_verts(8) {

	float3 v1(min.x, max.y, min.z);
	float3 v2(max.x, max.y, min.x);
	float3 v3(max.x, min.y, min.z);
	float3 v4(min.x, min.y, max.z);
	float3 v5(max.x, min.y, max.z);
	float3 v7(min.x, max.y, max.z);

	origin_verts[0] = XMLoadFloat3(&min);
	origin_verts[1] = XMLoadFloat3(&v1);
	origin_verts[2] = XMLoadFloat3(&v2);
	origin_verts[3] = XMLoadFloat3(&v3);

	origin_verts[4] = XMLoadFloat3(&v4);
	origin_verts[5] = XMLoadFloat3(&v5);
	origin_verts[6] = XMLoadFloat3(&max);
	origin_verts[7] = XMLoadFloat3(&v7);
}


AABB::AABB(FXMVECTOR min, FXMVECTOR max)
	: min_point(min)
	, max_point(max)
	, origin_verts(8)
	, transformed_verts(8) {

	float3 fmin, fmax;
	XMStoreFloat3(&fmin, min);
	XMStoreFloat3(&fmax, max);

	float3 v1(fmin.x, fmax.y, fmin.z);
	float3 v2(fmax.x, fmax.y, fmin.x);
	float3 v3(fmax.x, fmin.y, fmin.z);
	float3 v4(fmin.x, fmin.y, fmax.z);
	float3 v5(fmax.x, fmin.y, fmax.z);
	float3 v7(fmin.x, fmax.y, fmax.z);

	origin_verts[0] = min;
	origin_verts[1] = XMLoadFloat3(&v1);
	origin_verts[2] = XMLoadFloat3(&v2);
	origin_verts[3] = XMLoadFloat3(&v3);

	origin_verts[4] = XMLoadFloat3(&v4);
	origin_verts[5] = XMLoadFloat3(&v5);
	origin_verts[6] = max;
	origin_verts[7] = XMLoadFloat3(&v7);

	transformed_verts = origin_verts;
}


void XM_CALLCONV AABB::transform(FXMMATRIX M) {

	for (u32 i = 0; i < 8; ++i) {
		transformed_verts[i] = XMVector3TransformCoord(origin_verts[i], M);
	}

	const auto pair = MinMaxPoint(transformed_verts);
	min_point = pair.first;
	max_point = pair.second;
}


//----------------------------------------------------------------------------------
// BoundingSphere
//----------------------------------------------------------------------------------
BoundingSphere::BoundingSphere()
	: origin_center(XMVectorZero())
	, origin_radius(FLT_MAX)
	, transformed_radius(FLT_MAX)
	, transformed_center(XMVectorZero()) {
}


BoundingSphere::BoundingSphere(const float3& center, float radius)
	: origin_center(XMLoadFloat3(&center))
	, origin_radius(radius)
	, transformed_radius(radius)
	, transformed_center(XMLoadFloat3(&center)) {
}


BoundingSphere::BoundingSphere(FXMVECTOR center, float radius)
	: origin_center(center)
	, origin_radius(radius)
	, transformed_radius(radius)
	, transformed_center(center) {
}


void XM_CALLCONV BoundingSphere::transform(FXMMATRIX M) {

	const float x = XMVectorGetX(XMVector3Length(M.r[0]));
	const float y = XMVectorGetX(XMVector3Length(M.r[1]));
	const float z = XMVectorGetX(XMVector3Length(M.r[2]));

	transformed_radius *= std::max(std::max(x, y), z);
	transformed_center = XMVector3TransformCoord(origin_center, M);
}
