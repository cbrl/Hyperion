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

		void SetPosition(XMFLOAT3 newPosition) { 
			buffer.position = newPosition;
			position        = XMVectorSet(newPosition.x, newPosition.y, newPosition.z, 0.0f);
		}
		void SetRotation(XMFLOAT3 rotation) {
			pitch = rotation.x;
			yaw   = rotation.y;
			roll  = rotation.z;
		}

		void Move(XMFLOAT3 units);
		void Rotate(XMFLOAT3 units);
		void Update(float deltaTime);

		CameraBuffer GetBuffer()     { return buffer; }
		XMMATRIX     GetViewMatrix() { return viewMatrix; }
		XMFLOAT3     GetPosition()   { return buffer.position; }
		XMFLOAT3     GetRotation()   { return XMFLOAT3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll)); }
		XMFLOAT3     GetVelocity()   { return velocity; }

	private:
		CameraBuffer buffer;
		XMMATRIX     viewMatrix;

		const XMVECTOR   defaultForward;
		const XMVECTOR   defaultRight;
		const XMVECTOR   defaultUp;

		XMVECTOR    cameraForward;
		XMVECTOR    fpsForward;
		XMVECTOR    cameraRight;
		XMVECTOR    cameraUp;
		XMVECTOR    lookAt;
		XMVECTOR    position;

		XMFLOAT3    moveUnits;
		XMFLOAT3    velocity;
		const float moveAccel;
		const float moveDecel;
		const float maxVelocity;

		float       pitch;
		float       yaw;
		float       roll;
		const float maxPitch;
		const float turnFactor;

		bool enableFreeLook;
		bool fpsMode;
		bool isMoving;
};

