#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <math.h>
#include "rendering\buffer\Buffers.h"

using namespace DirectX;

class Camera {
	public:
		Camera();
		~Camera();

		void SetPosition(XMFLOAT3 position);
		void SetRotation(XMFLOAT3 rotation);

		void Move(XMFLOAT3 speeds);
		void Rotate(XMFLOAT3 speeds);

		void Render();

		CameraBuffer GetBuffer();
		XMFLOAT3     GetPosition();
		XMFLOAT3     GetRotation();
		XMMATRIX     GetViewMatrix();

	private:
		CameraBuffer   m_Buffer;
		XMFLOAT3       m_Rotation;
		XMMATRIX       m_ViewMatrix;
		const XMVECTOR m_DefaultForward;
		const XMVECTOR m_DefaultRight;
		XMVECTOR       m_CameraForward;
		XMVECTOR       m_CameraRight;
		XMVECTOR       m_CameraUp;

		XMFLOAT3 m_MoveSpeed;
		XMFLOAT3 m_TurnSpeed;
};

