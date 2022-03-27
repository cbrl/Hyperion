module;

#include "datatypes/vector_types.h"
#include "directxmath/directxmath.h"

export module math.geometry;

export import math.geometry.bounding_volume;
export import math.geometry.frustum;
export import math.geometry.transform_3d;
export import math.geometry.shapes;


export {

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

} //export
