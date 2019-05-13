#pragma once

#include "bounding_volume/bounding_volume.h"
#include "frustum/frustum.h"
#include "transform/transform_3d.h"


// Check if a ray intersects an AABB
[[nodiscard]]
bool intersects(const f32_3& ray_origin, const f32_3& ray_direction, const AABB& aabb);

// Check if a ray intersects an AABB
[[nodiscard]]
bool XM_CALLCONV intersects(FXMVECTOR ray_origin, FXMVECTOR ray_direction, const AABB& aabb);


// Check if a ray intersects a BoundingSphere
[[nodiscard]]
bool intersects(const f32_3& ray_origin, const f32_3& ray_direction, const BoundingSphere& sphere);


// Check if a ray intersects an BoundingSphere
[[nodiscard]]
bool XM_CALLCONV intersects(XMVECTOR ray_origin, XMVECTOR ray_direction, const BoundingSphere& sphere);