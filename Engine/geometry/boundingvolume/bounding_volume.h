#pragma once

#include "util\math\math.h"
#include "util\datatypes\datatypes.h"

using namespace DirectX;

struct AABB {
	public:
		AABB() : min_point(g_XMInfinity), max_point(g_XMNegInfinity) {}
		~AABB() = default;

		AABB(float3 min, float3 max)
			: min_point(XMLoadFloat3(&min)), max_point(XMLoadFloat3(&max)) {

			vertices.push_back(XMLoadFloat3(&min));
			vertices.push_back(XMLoadFloat3(&float3(min.x, max.y, min.z)));
			vertices.push_back(XMLoadFloat3(&float3(max.x, max.y, min.x)));
			vertices.push_back(XMLoadFloat3(&float3(max.x, min.y, min.z)));

			vertices.push_back(XMLoadFloat3(&float3(min.x, min.y, max.z)));
			vertices.push_back(XMLoadFloat3(&float3(max.x, min.y, max.z)));
			vertices.push_back(XMLoadFloat3(&max));
			vertices.push_back(XMLoadFloat3(&float3(min.x, max.y, max.z)));
		}

		AABB(XMVECTOR min, XMVECTOR max)
			: min_point(min), max_point(max) {

			float3 fmin, fmax;
			XMStoreFloat3(&fmin, min);
			XMStoreFloat3(&fmax, max);

			vertices.push_back(XMLoadFloat3(&fmin));
			vertices.push_back(XMLoadFloat3(&float3(fmin.x, fmax.y, fmin.z)));
			vertices.push_back(XMLoadFloat3(&float3(fmax.x, fmax.y, fmin.x)));
			vertices.push_back(XMLoadFloat3(&float3(fmax.x, fmin.y, fmin.z)));

			vertices.push_back(XMLoadFloat3(&float3(fmin.x, fmin.y, fmax.z)));
			vertices.push_back(XMLoadFloat3(&float3(fmax.x, fmin.y, fmax.z)));
			vertices.push_back(XMLoadFloat3(&fmax));
			vertices.push_back(XMLoadFloat3(&float3(fmin.x, fmax.y, fmax.z)));
		}

		const XMVECTOR Min() const { return min_point; }
		const XMVECTOR Max() const { return max_point; }

		void Transform(XMMATRIX M) {
			XMVECTOR transformed_verts[8];

			for (i32 i = 0; i < 8; ++i) {
				transformed_verts[i] = XMVector3TransformCoord(vertices[i], M);
			}

			auto pair = MinMaxPoint(transformed_verts, 8);
			min_point = pair.first;
			max_point = pair.second;
		}


	private:
		XMVECTOR min_point;
		XMVECTOR max_point;

		vector<XMVECTOR> vertices;
};


struct BoundingSphere {
	BoundingSphere() : center(XMVectorZero()), radius(FLT_MAX) {}
	~BoundingSphere() = default;

	BoundingSphere(float3 center, float radius)
		: center(XMLoadFloat3(&center))
		, radius(radius)
	{}

	BoundingSphere(XMVECTOR center, float radius)
		: center(center)
		, radius(radius)
	{}


	const XMVECTOR& Center() const { return center; }
	const float&    Radius() const { return radius; }


	private:
		XMVECTOR center;
		float    radius;
};