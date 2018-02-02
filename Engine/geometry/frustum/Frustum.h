#include <d3d11.h>

using namespace DirectX;

class Frustum {
	public:
		Frustum(XMMATRIX viewProj);
		~Frustum();

		bool ContainsPoint(XMFLOAT3 point);
		bool ContainsSphere(XMFLOAT3 center, float radius);
		bool ContainsCube(XMFLOAT3 center, float radius);
		bool ContainsBox(XMFLOAT3 center, XMFLOAT3 xyzSize);

	private:
		XMVECTOR planes[6];
};