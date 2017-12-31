#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "rendering\buffer\BufferTypes.h"

using namespace DirectX;

class Camera {
	public:
		Camera();
		~Camera();

		void SetPosition(XMFLOAT3 position);
		void SetRotation(XMFLOAT3 rotation);
		void Render();

		CameraBuffer GetBuffer();
		XMFLOAT3     GetPosition();
		XMFLOAT3     GetRotation();
		XMMATRIX     GetViewMatrix();

	private:
		CameraBuffer m_Buffer;
		XMFLOAT3     m_Position;
		XMFLOAT3     m_Rotation;
		XMMATRIX     m_ViewMatrix;
};

