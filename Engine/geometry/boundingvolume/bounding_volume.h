#pragma once

#include "util\math\math.h"
#include "util\datatypes\datatypes.h"

using namespace DirectX;

struct AABB final {
	public:
		AABB()
			: min_point(g_XMInfinity)
			, max_point(g_XMNegInfinity)
			, vertices(8) 
		{}

		~AABB() = default;

		// Construct an AABB from the min and max points on an object
		AABB(const float3& min, const float3& max)
			: min_point(XMLoadFloat3(&min))
			, max_point(XMLoadFloat3(&max))
			, vertices(8) {

			vertices[0] = XMLoadFloat3(&min);
			vertices[1] = XMLoadFloat3(&float3(min.x, max.y, min.z));
			vertices[2] = XMLoadFloat3(&float3(max.x, max.y, min.x));
			vertices[3] = XMLoadFloat3(&float3(max.x, min.y, min.z));

			vertices[4] = XMLoadFloat3(&float3(min.x, min.y, max.z));
			vertices[5] = XMLoadFloat3(&float3(max.x, min.y, max.z));
			vertices[6] = XMLoadFloat3(&max);
			vertices[7] = XMLoadFloat3(&float3(min.x, max.y, max.z));
		}

		// Construct an AABB from the min and max points on an object
		AABB(FXMVECTOR min, FXMVECTOR max)
			: min_point(min)
			, max_point(max)
			, vertices(8) {

			float3 fmin, fmax;
			XMStoreFloat3(&fmin, min);
			XMStoreFloat3(&fmax, max);

			vertices[0] = XMLoadFloat3(&fmin);
			vertices[1] = XMLoadFloat3(&float3(fmin.x, fmax.y, fmin.z));
			vertices[2] = XMLoadFloat3(&float3(fmax.x, fmax.y, fmin.x));
			vertices[3] = XMLoadFloat3(&float3(fmax.x, fmin.y, fmin.z));

			vertices[4] = XMLoadFloat3(&float3(fmin.x, fmin.y, fmax.z));
			vertices[5] = XMLoadFloat3(&float3(fmax.x, fmin.y, fmax.z));
			vertices[6] = XMLoadFloat3(&fmax);
			vertices[7] = XMLoadFloat3(&float3(fmin.x, fmax.y, fmax.z));
		}


		// Transform the AABB by a matrix
		void XM_CALLCONV Transform(FXMMATRIX M) {
			for (u32 i = 0; i < 8; ++i) {
				vertices[i] = XMVector3TransformCoord(vertices[i], M);
			}

			auto pair = MinMaxPoint(vertices);
			min_point = pair.first;
			max_point = pair.second;
		}


		const XMVECTOR& Min() const { return min_point; }
		const XMVECTOR& Max() const { return max_point; }


	private:
		XMVECTOR min_point;
		XMVECTOR max_point;

		vector<XMVECTOR> vertices;
};


struct BoundingSphere final {
	BoundingSphere() : center(XMVectorZero()), radius(FLT_MAX) {}
	~BoundingSphere() = default;

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(const float3& center, float radius)
		: center(XMLoadFloat3(&center))
		, radius(radius)
	{}

	// Construct a bounding sphere from the center point and radius of an object
	BoundingSphere(FXMVECTOR center, float radius)
		: center(center)
		, radius(radius)
	{}


	// Transform the bounding sphere by a matrix
	void XM_CALLCONV Transform(FXMMATRIX M) {
		float x = XMVectorGetX(XMVector3Length(M.r[0]));
		float y = XMVectorGetX(XMVector3Length(M.r[1]));
		float z = XMVectorGetX(XMVector3Length(M.r[2]));

		radius *= std::max(std::max(x, y), z);
		center  = XMVector3TransformCoord(center, M);
	}


	const XMVECTOR& Center() const { return center; }
	const float     Radius() const { return radius; }


	private:
		XMVECTOR center;
		float    radius;
};