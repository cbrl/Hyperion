#pragma once

#include <math.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "rendering\buffer\buffers.h"

using namespace DirectX;

class Camera {
	public:
		Camera();
		~Camera();

		void SetPosition(XMFLOAT3 newPosition) { 
			position = XMVectorSet(newPosition.x, newPosition.y, newPosition.z, 0.0f);
		}
		void SetRotation(XMFLOAT3 rotation) {
			pitch = rotation.x;
			yaw   = rotation.y;
			roll  = rotation.z;
		}

		void Move(XMFLOAT3 units);
		void Rotate(XMFLOAT3 units);
		void Update(float deltaTime);

		XMMATRIX GetViewMatrix() const { return view_matrix; }
		XMFLOAT3 GetRotation()   const { return XMFLOAT3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll)); }
		XMFLOAT3 GetVelocity()   const { return velocity; }
		XMFLOAT3 GetPosition()   const {
			XMFLOAT3 pos;
			XMStoreFloat3(&pos, position);
			return pos;
		}

	private:
		XMMATRIX view_matrix;

		const XMVECTOR   default_forward;
		const XMVECTOR   default_right;
		const XMVECTOR   default_up;

		XMVECTOR    camera_forward;
		XMVECTOR    fps_forward;
		XMVECTOR    camera_right;
		XMVECTOR    camera_up;
		XMVECTOR    look_at;
		XMVECTOR    position;

		XMFLOAT3    move_units;
		XMFLOAT3    velocity;
		const float move_accel;
		const float move_decel;
		const float max_velocity;

		float       pitch;
		float       yaw;
		float       roll;
		const float max_pitch;
		const float turn_factor;

		bool enable_free_look;
		bool fps_mode;
		bool is_moving;
};

