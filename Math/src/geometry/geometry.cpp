module;

#include "datatypes/vector_types.h"
#include "directxmath/directxmath.h"

module math.geometry;

import math.geometry.bounding_volume;


bool intersects(const f32_3& ray_origin, const f32_3& ray_direction, const BoundingSphere& sphere) {
	const f32 a = std::pow(ray_direction[0], 2)
	              + std::pow(ray_direction[1], 2)
	              + std::pow(ray_direction[2], 2);

	const f32 b = 2.0f * ( (ray_direction[0] * ray_origin[0])
	                       + (ray_direction[1] * ray_origin[1])
	                       + (ray_direction[2] * ray_origin[2]) );

	const f32 c = std::pow(ray_origin[0], 2)
	              + std::pow(ray_origin[1], 2)
	              + std::pow(ray_origin[2], 2)
	              - std::pow(sphere.radius(), 2);

	const f32 discriminant = (b * b) - (4 * a * c);

	return discriminant >= 0.0f;
}


bool XM_CALLCONV intersects(XMVECTOR ray_origin, XMVECTOR ray_direction, const BoundingSphere& sphere) {
	ray_origin = XMVectorSetW(ray_origin, 0.0f);
	ray_direction = XMVectorSetW(ray_direction, 0.0f);

	const f32 a = XMVectorGetW(XMVectorSum(ray_direction * ray_direction));
	const f32 b = XMVectorGetW(XMVectorSum(ray_origin * ray_direction * 2.0f));
	const f32 c = XMVectorGetW(XMVectorSum(ray_origin * ray_origin)) - std::pow(sphere.radius(), 2);

	const f32 discriminant = (b * b) - (4 * a * c);

	return discriminant >= 0.0f;
}


bool intersects(const f32_3& ray_origin, const f32_3& ray_direction, const AABB& aabb) {
	const f32_3 min = XMStore<f32_3>(aabb.min());
	const f32_3 max = XMStore<f32_3>(aabb.max());
	
	f32 tmin = -std::numeric_limits<f32>::infinity();
	f32 tmax = std::numeric_limits<f32>::infinity();

	if (ray_direction[0] != 0.0f) {
		const f32 tx1 = (min[0] - ray_origin[0]) / ray_direction[0];
		const f32 tx2 = (max[0] - ray_origin[0]) / ray_direction[0];

		tmin = std::max(tmin, std::min(tx1, tx2));
		tmax = std::min(tmax, std::max(tx1, tx2));
	}

	if (ray_direction[1] != 0.0f) {
		const f32 ty1 = (min[1] - ray_origin[1]) / ray_direction[1];
		const f32 ty2 = (max[1] - ray_origin[1]) / ray_direction[1];

		tmin = std::max(tmin, std::min(ty1, ty2));
		tmax = std::min(tmax, std::max(ty1, ty2));
	}

	if (ray_direction[2] != 0.0f) {
		const f32 tz1 = (min[2] - ray_origin[2]) / ray_direction[2];
		const f32 tz2 = (max[2] - ray_origin[2]) / ray_direction[2];

		tmin = std::max(tmin, std::min(tz1, tz2));
		tmax = std::min(tmax, std::max(tz1, tz2));
	}

	return tmax >= tmin;
}


bool XM_CALLCONV intersects(FXMVECTOR ray_origin, FXMVECTOR ray_direction, const AABB& aabb) {
	f32 tmin = -std::numeric_limits<f32>::infinity();
	f32 tmax = std::numeric_limits<f32>::infinity();

	const XMVECTOR t1 = (aabb.min() - ray_origin) / ray_direction;
	const XMVECTOR t2 = (aabb.max() - ray_origin) / ray_direction;
	
	const f32_3 tmin1 = XMStore<f32_3>(XMVectorMin(t1, t2));
	const f32_3 tmax1 = XMStore<f32_3>(XMVectorMax(t1, t2));

	tmin = *std::max_element(tmin1.begin(), tmin1.end());
	tmax = *std::min_element(tmax1.begin(), tmax1.end());

	return tmax >= tmin;
}
