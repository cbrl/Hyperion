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
			m_Position = XMVectorSet(position.x, position.y, position.z, 0.0f);
		}
		void SetRotation(XMFLOAT3 rotation) { m_Rotation = rotation; }

		void Move(XMINT3 directions, float deltaTime);
		void Rotate(XMINT3 directions, float deltaTime);
		void Update();

		CameraBuffer GetBuffer()     { return m_Buffer; }
		XMFLOAT3     GetPosition()   { return m_Buffer.position; }
		XMFLOAT3     GetRotation()   { return XMFLOAT3(XMConvertToDegrees(m_Pitch), XMConvertToDegrees(m_Yaw), XMConvertToDegrees(m_Roll)); }
		XMMATRIX     GetViewMatrix() { return m_ViewMatrix; }

	private:
		CameraBuffer m_Buffer;
		XMFLOAT3     m_Rotation;
		XMMATRIX     m_ViewMatrix;

		const XMVECTOR   m_DefaultForward;
		const XMVECTOR   m_DefaultRight;
		const XMVECTOR   m_DefaultUp;

		XMVECTOR    m_CameraForward;
		XMVECTOR    m_CameraRight;
		XMVECTOR    m_CameraUp;
		XMVECTOR    m_LookAt;
		XMVECTOR    m_Position;

		XMFLOAT3    m_MoveSpeed;
		const float m_MoveAccel;
		const float m_MoveDecel;
		const float m_MaxMoveSpeed;

		float       m_Pitch;
		float       m_Yaw;
		float       m_Roll;
		float       m_TurnSpeed;
		const float m_MaxPitch;
		const float m_TurnFactor;

		bool m_EnableFreeLook;
};

