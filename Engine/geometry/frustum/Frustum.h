#pragma once

#include "util\datatypes\datatypes.h"
#include "geometry\boundingvolume\bounding_volume.h"

using namespace DirectX;

class Frustum {
	public:
		Frustum() = default;
		Frustum(const XMMATRIX& viewProj);
		~Frustum();

		void CreateFrustum(const XMMATRIX& viewProj);

		bool Encloses(const XMVECTOR& point) const;
		bool Encloses(const AABB& aabb) const;
		bool Contains(const AABB& aabb) const;


	private:
		XMVECTOR planes[6];
};