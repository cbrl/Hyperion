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

		void SetPosition(XMFLOAT3 position) { m_Buffer.position = position; }
		void SetRotation(XMFLOAT3 rotation) { m_Rotation = rotation; }

		void Move(XMINT3 directions, float deltaTime);
		void Rotate(XMINT3 directions, float deltaTime);

		void Update();

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

		XMFLOAT3    m_MoveSpeed;
		const float m_MoveAccel;
		const float m_MoveDecel;
		const float m_MaxMoveSpeed;

		XMFLOAT3    m_TurnSpeed;
		const float m_TurnAccel;
		const float m_TurnDecel;
		const float m_MaxTurnSpeed;
};

