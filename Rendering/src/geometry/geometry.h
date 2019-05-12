#pragma once

#include "geometry_types.h"
#include "bounding_volume/bounding_volume.h"
#include "frustum/frustum.h"
#include "transform/transform_3d.h"


// Check if a ray intersects an AABB
[[nodiscard]]
bool intersects(const Ray3& ray, const AABB& aabb);


// Check if a ray intersects a BoundingSphere
[[nodiscard]]
bool intersects(const Ray3& ray, const BoundingSphere& sphere);