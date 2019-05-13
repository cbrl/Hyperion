#include "geometry.h"


namespace {

bool intersects(const f32_3& origin, const f32_3& direction, const BoundingSphere& sphere) {
	const f32 a = std::pow(direction[0], 2)
	              + std::pow(direction[1], 2)
	              + std::pow(direction[2], 2);

	const f32 b = 2.0f * ( (direction[0] * origin[0])
	                       + (direction[1] * origin[1])
	                       + (direction[2] * origin[2]) );

	const f32 c = std::pow(origin[0], 2)
	              + std::pow(origin[1], 2)
	              + std::pow(origin[2], 2)
	              - std::pow(sphere.radius(), 2);

	const f32 discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0.0f)
		return false;

	return true;
}


bool XM_CALLCONV intersects(XMVECTOR origin, XMVECTOR direction, const BoundingSphere& sphere) {
	origin = XMVectorSetW(origin, 0.0f);
	direction = XMVectorSetW(direction, 0.0f);

	const f32 a = XMVectorGetW(XMVectorSum(direction * direction));
	const f32 b = XMVectorGetW(XMVectorSum(origin * direction * 2.0f));
	const f32 c = XMVectorGetW(XMVectorSum(origin * origin)) - std::pow(sphere.radius(), 2);

	const f32 discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0.0f)
		return false;

	return true;
}


bool intersects(const f32_3& origin, const f32_3& direction, const AABB& aabb) {
	const f32_3 min = XMStore<f32_3>(aabb.min());
	const f32_3 max = XMStore<f32_3>(aabb.max());
	
	f32 tmin = -std::numeric_limits<f32>::infinity();
	f32 tmax = std::numeric_limits<f32>::infinity();

	if (direction[0] != 0.0f) {
		const f32 tx1 = (min[0] - origin[0]) / direction[0];
		const f32 tx2 = (max[0] - origin[0]) / direction[0];

		tmin = std::max(tmin, std::min(tx1, tx2));
		tmax = std::min(tmax, std::max(tx1, tx2));
	}

	if (direction[1] != 0.0f) {
		const f32 ty1 = (min[1] - origin[1]) / direction[1];
		const f32 ty2 = (max[1] - origin[1]) / direction[1];

		tmin = std::max(tmin, std::min(ty1, ty2));
		tmax = std::min(tmax, std::max(ty1, ty2));
	}

	if (direction[2] != 0.0f) {
		const f32 tz1 = (min[2] - origin[2]) / direction[2];
		const f32 tz2 = (max[2] - origin[2]) / direction[2];

		tmin = std::max(tmin, std::min(tz1, tz2));
		tmax = std::min(tmax, std::max(tz1, tz2));
	}

	return tmax >= tmin;
}


bool XM_CALLCONV intersects(FXMVECTOR origin, FXMVECTOR direction, const AABB& aabb) {
	f32 tmin = -std::numeric_limits<f32>::infinity();
	f32 tmax = std::numeric_limits<f32>::infinity();

	const XMVECTOR t1 = (aabb.min() - origin) / direction;
	const XMVECTOR t2 = (aabb.max() - origin) / direction;
	
	const f32_3 tmin1 = XMStore<f32_3>(XMVectorMin(t1, t2));
	const f32_3 tmax1 = XMStore<f32_3>(XMVectorMax(t1, t2));

	tmin = *std::max_element(tmin1.begin(), tmin1.end());
	tmax = *std::min_element(tmax1.begin(), tmax1.end());

	return tmax >= tmin;
}

} //namespace


bool intersects(const Ray3& ray, const AABB& aabb) {
	const XMVECTOR origin    = XMLoad(&ray.getOrigin());
	const XMVECTOR direction = XMLoad(&ray.getDirection());

	return intersects(origin, direction, aabb);
}


bool intersects(const Ray3& ray, const BoundingSphere& sphere) {
	const XMVECTOR origin    = XMLoad(&ray.getOrigin());
	const XMVECTOR direction = XMLoad(&ray.getDirection());

	return intersects(origin, direction, sphere);
}