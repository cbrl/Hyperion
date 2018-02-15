#pragma once

#include <math.h>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

struct AABB {
	public:
		AABB() : minPoint(g_XMInfinity), maxPoint(-g_XMInfinity) {}

		AABB(XMFLOAT3 min, XMFLOAT3 max)
			: minPoint(XMLoadFloat3(&min)), maxPoint(XMLoadFloat3(&max)) {}

		AABB(XMVECTOR min, XMVECTOR max)
			: minPoint(min), maxPoint(max) {}

		AABB(const std::vector<XMFLOAT3>& vertices) 
			: minPoint(g_XMInfinity), maxPoint(-g_XMInfinity)
		{
			XMFLOAT3 min, max;
			XMStoreFloat3(&min, minPoint);
			XMStoreFloat3(&max, maxPoint);

			for (size_t i = 0; i < vertices.size(); ++i) {
				min.x = std::fminf(min.x, vertices[i].x);
				min.y = std::fminf(min.y, vertices[i].y);
				min.z = std::fminf(min.z, vertices[i].y);

				max.x = std::fmaxf(max.x, vertices[i].x);
				max.y = std::fmaxf(max.y, vertices[i].y);
				max.z = std::fmaxf(max.z, vertices[i].y);
			}

			minPoint = XMLoadFloat3(&min);
			maxPoint = XMLoadFloat3(&max);
		}

		~AABB() = default;

		XMVECTOR Min() const { return minPoint; }
		XMVECTOR Max() const { return maxPoint; }

		void SetMin(XMVECTOR min) { minPoint = min; }
		void SetMin(XMFLOAT3 min) { minPoint = XMLoadFloat3(&min); }
		void SetMax(XMVECTOR max) { maxPoint = max; }
		void SetMax(XMFLOAT3 max) { maxPoint = XMLoadFloat3(&max); }


	private:
		XMVECTOR minPoint;
		XMVECTOR maxPoint;
};