#pragma once

#include <math.h>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

struct AABB {
	public:
		AABB() : min_point(g_XMInfinity), max_point(-g_XMInfinity) {}

		AABB(XMFLOAT3 min, XMFLOAT3 max)
			: min_point(XMLoadFloat3(&min)), max_point(XMLoadFloat3(&max)) {}

		AABB(XMVECTOR min, XMVECTOR max)
			: min_point(min), max_point(max) {}

		AABB(const std::vector<XMFLOAT3>& vertices) 
			: min_point(g_XMInfinity), max_point(-g_XMInfinity)
		{
			XMFLOAT3 min, max;
			XMStoreFloat3(&min, min_point);
			XMStoreFloat3(&max, max_point);

			for (size_t i = 0; i < vertices.size(); ++i) {
				min.x = std::fminf(min.x, vertices[i].x);
				min.y = std::fminf(min.y, vertices[i].y);
				min.z = std::fminf(min.z, vertices[i].y);

				max.x = std::fmaxf(max.x, vertices[i].x);
				max.y = std::fmaxf(max.y, vertices[i].y);
				max.z = std::fmaxf(max.z, vertices[i].y);
			}

			min_point = XMLoadFloat3(&min);
			max_point = XMLoadFloat3(&max);
		}

		~AABB() = default;

		XMVECTOR Min() const { return min_point; }
		XMVECTOR Max() const { return max_point; }

		void SetMin(XMVECTOR min) { min_point = min; }
		void SetMin(XMFLOAT3 min) { min_point = XMLoadFloat3(&min); }
		void SetMax(XMVECTOR max) { max_point = max; }
		void SetMax(XMFLOAT3 max) { max_point = XMLoadFloat3(&max); }


	private:
		XMVECTOR min_point;
		XMVECTOR max_point;
};