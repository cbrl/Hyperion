module;

#include <DirectXMath.h>

#include "datatypes/vector_types.h"

export module math.geometry;

export import :bounding_volume;
export import :frustum;
export import :transform_3d;
export import :shapes;

using namespace DirectX;

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
