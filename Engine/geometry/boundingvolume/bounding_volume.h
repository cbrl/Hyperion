#pragma once

#include "util\math\math.h"
#include "util\datatypes\datatypes.h"

using namespace DirectX;

struct AABB final {
	public:
		AABB()
			: min_point(g_XMInfinity)
			, max_point(g_XMNegInfinity)
			, origin_verts(8)
			, transformed_verts(8)
		{}

		~AABB() = default;

		// Construct an AABB from the min and max points on an object
		AABB(const float3& min, const float3& max)
			: min_point(XMLoadFloat3(&min))
			, max_point(XMLoadFloat3(&max))
			, origin_verts(8)
			, transformed_verts(8)
		{
			origin_verts[0] = XMLoadFloat3(&min);
			origin_verts[1] = XMLoadFloat3(&float3(min.x, max.y, min.z));
			origin_verts[2] = XMLoadFloat3(&float3(max.x, max.y, min.x));
			origin_verts[3] = XMLoadFloat3(&float3(max.x, min.y, min.z));

			origin_verts[4] = XMLoadFloat3(&float3(min.x, min.y, max.z));
			origin_verts[5] = XMLoadFloat3(&float3(max.x, min.y, max.z));
			origin_verts[6] = XMLoadFloat3(&max);
			origin_verts[7] = XMLoadFloat3(&float3(min.x, max.y, max.z));
		}

		// Construct an AABB from the min and max points on an object
		AABB(FXMVECTOR min, FXMVECTOR max)
			: min_point(min)
			, max_point(max)
			, origin_verts(8)
			, transformed_verts(8)
		{
			float3 fmin, fmax;
			XMStoreFloat3(&fmin, min);
			XMStoreFloat3(&fmax, max);

			origin_verts[0] = XMLoadFloat3(&fmin);
			origin_verts[1] = XMLoadFloat3(&float3(fmin.x, fmax.y, fmin.z));
			origin_verts[2] = XMLoadFloat3(&float3(fmax.x, fmax.y, fmin.x));
			origin_verts[3] = XMLoadFloat3(&float3(fmax.x, fmin.y, fmin.z));

			origin_verts[4] = XMLoadFloat3(&float3(fmin.x, fmin.y, fmax.z));
			origin_verts[5] = XMLoadFloat3(&float3(fmax.x, fmin.y, fmax.z));
			origin_verts[6] = XMLoadFloat3(&fmax);
			origin_verts[7] = XMLoadFloat3(&float3(fmin.x, fmax.y, fmax.z));

			transformed_verts = origin_verts;
		}


		// Transform the AABB by a matrix
		void XM_CALLCONV Transform(FXMMATRIX M) {
			for (u32 i = 0; i < 8; ++i) {
				transformed_verts[i] = XMVector3Transform(origin_verts[i], M);
			}

			auto pair = MinMaxPoint(transformed_verts);
			min_point = pair.first;
			max_point = pair.second;
		}


		const XMVECTOR& Min() const { return min_point; }
		const XMVECTOR& Max() const { return max_point; }


	private:
		XMVECTOR min_point;
		XMVECTOR max_point;

		vector<XMVECTOR> origin_verts;
		vector<XMVECTOR> transformed_verts;
};


struct BoundingSphere final {
	BoundingSphere()
		: origin_center(XMVectorZero())
		, origin_radius(FLT_MAX)
		, transformed_center(XMVectorZero())
		, transformed_radius(FLT_MAX)
	{}
	~BoundingSphere() = default;

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(const float3& center, float radius)
		: origin_center(XMLoadFloat3(&center))
		, origin_radius(radius)
		, transformed_center(XMLoadFloat3(&center))
		, transformed_radius(radius)
	{}

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(FXMVECTOR center, float radius)
		: origin_center(center)
		, origin_radius(radius)
		, transformed_center(center)
		, transformed_radius(radius)
	{}


	// Transform the bounding sphere by a matrix
	void XM_CALLCONV Transform(FXMMATRIX M) {
		float x = XMVectorGetX(XMVector3Length(M.r[0]));
		float y = XMVectorGetX(XMVector3Length(M.r[1]));
		float z = XMVectorGetX(XMVector3Length(M.r[2]));

		transformed_radius *= std::max(std::max(x, y), z);
		transformed_center = XMVector3TransformCoord(origin_center, M);
	}


	const XMVECTOR& Center() const { return transformed_center; }
	const float     Radius() const { return transformed_radius; }


	private:
		XMVECTOR origin_center;
		float    origin_radius;

		float    transformed_radius;
		XMVECTOR transformed_center;
};