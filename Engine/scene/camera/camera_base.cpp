#include "stdafx.h"
#include "camera_base.h"


CameraBase::CameraBase(ID3D11Device* device,
					   ID3D11DeviceContext* device_context,
					   u32 viewport_width,
					   u32 viewport_height)
	: buffer(device)
	, viewport({ 0.0f, 0.0f, 0.0f, 0.f, 0.0f, 1.0f })
	, position(XMVectorZero())
	, pitch(0.0f)
	, yaw(0.0f)
	, roll(0.0f)

	, camera_forward(default_forward)
	, camera_right(default_right)
	, camera_up(default_up)

	, fov(XM_PI / 4.0f)
	, view_matrix(XMMatrixIdentity())
	, projection_matrix(XMMatrixIdentity())

	, z_near(0.1f)
	, z_far(1000.0f) {
}
