#include "bounding_volume.h"


//----------------------------------------------------------------------------------
// AABB
//----------------------------------------------------------------------------------
AABB::AABB()
	: min_point(g_XMInfinity)
	, max_point(g_XMNegInfinity) {
}


AABB::AABB(const float3& min, const float3& max)
	: min_point(XMLoadFloat3(&min))
	, max_point(XMLoadFloat3(&max)) {
}


AABB::AABB(FXMVECTOR min, FXMVECTOR max)
	: min_point(min)
	, max_point(max) {
}



//----------------------------------------------------------------------------------
// BoundingSphere
//----------------------------------------------------------------------------------
BoundingSphere::BoundingSphere()
	: sphere_center(XMVectorZero())
	, sphere_radius(FLT_MAX) {
}


BoundingSphere::BoundingSphere(const float3& center, float radius)
	: sphere_center(XMLoadFloat3(&center))
	, sphere_radius(radius) {
}


BoundingSphere::BoundingSphere(FXMVECTOR center, float radius)
	: sphere_center(center)
	, sphere_radius(radius) {
}
