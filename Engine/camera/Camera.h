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

		void SetPosition(XMFLOAT3 position) { 
			m_Buffer.position = position; 
			m_Position        = XMVectorSet(position.x, position.y, position.z, 0.0f);
		}
		void SetRotation(XMFLOAT3 rotation) {
			m_Pitch = rotation.x;
			m_Yaw   = rotation.y;
			m_Roll  = rotation.z;
		}

		void Move(XMFLOAT3 units);
		void Rotate(XMFLOAT3 units);
		void Update(float deltaTime);

		CameraBuffer GetBuffer()     { return m_Buffer; }
		XMMATRIX     GetViewMatrix() { return m_ViewMatrix; }
		XMFLOAT3     GetPosition()   { return m_Buffer.position; }
		XMFLOAT3     GetRotation()   { return XMFLOAT3(XMConvertToDegrees(m_Pitch), XMConvertToDegrees(m_Yaw), XMConvertToDegrees(m_Roll)); }
		XMFLOAT3     GetVelocity()   { return m_Velocity; }

	private:
		CameraBuffer m_Buffer;
		XMMATRIX     m_ViewMatrix;

		const XMVECTOR   m_DefaultForward;
		const XMVECTOR   m_DefaultRight;
		const XMVECTOR   m_DefaultUp;

		XMVECTOR    m_CameraForward;
		XMVECTOR    m_CameraRight;
		XMVECTOR    m_CameraUp;
		XMVECTOR    m_LookAt;
		XMVECTOR    m_Position;

		XMFLOAT3    m_MoveUnits;
		XMFLOAT3    m_Velocity;
		const float m_MoveAccel;
		const float m_MoveDecel;
		const float m_MaxVelocity;

		float       m_Pitch;
		float       m_Yaw;
		float       m_Roll;
		const float m_MaxPitch;
		const float m_TurnFactor;

		bool m_EnableFreeLook;
		bool m_IsMoving;
};

