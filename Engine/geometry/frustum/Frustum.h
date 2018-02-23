#include <d3d11.h>
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

		//bool ContainsPoint(XMFLOAT3 point);
		//bool ContainsSphere(XMFLOAT3 center, float radius);
		//bool ContainsCube(XMFLOAT3 center, float radius);
		//bool ContainsBox(XMFLOAT3 center, XMFLOAT3 xyzSize);

	private:
		XMVECTOR planes[6];
};