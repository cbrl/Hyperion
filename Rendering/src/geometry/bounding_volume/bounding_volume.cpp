#include "bounding_volume.h"


//----------------------------------------------------------------------------------
// AABB
//----------------------------------------------------------------------------------
AABB::AABB()
	: min_point(g_XMInfinity)
	, max_point(g_XMNegInfinity) {
}


AABB::AABB(const vec3_f32& min, const vec3_f32& max)
	: min_point(XMLoad(&min))
	, max_point(XMLoad(&max)) {
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


BoundingSphere::BoundingSphere(const vec3_f32& center, f32 radius)
	: sphere_center(XMLoad(&center))
	, sphere_radius(radius) {
}


BoundingSphere::BoundingSphere(FXMVECTOR center, f32 radius)
	: sphere_center(center)
	, sphere_radius(radius) {
}
