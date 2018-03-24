#pragma once

#include "util\datatypes\datatypes.h"
#include "geometry\boundingvolume\bounding_volume.h"

using namespace DirectX;

class Frustum {
	public:
		Frustum() = default;
		Frustum(const XMMATRIX& viewProj);
		~Frustum();

		void UpdateFrustum(const XMMATRIX& viewProj);

		bool Encloses(const XMVECTOR& point) const;

		bool Encloses(const AABB& aabb) const;
		bool Contains(const AABB& aabb) const;

		bool Encloses(const BoundingSphere& sphere) const;
		bool Contains(const BoundingSphere& sphere) const;


	private:
		XMVECTOR planes[6];
};