#include "perspective_camera.h"


PerspectiveCamera::PerspectiveCamera(ID3D11Device& device, vec2_u32 viewport_size)
	: CameraBase(device)
	, fov(XM_PI / 4.0f) {

	viewport.setSize(viewport_size);
}


XMMATRIX XM_CALLCONV PerspectiveCamera::getCameraToProjectionMatrix() const {

	const vec2_u32 size    = viewport.getSize();
	const f32 aspect_ratio = static_cast<f32>(size.x) / static_cast<f32>(size.y);

	return XMMatrixPerspectiveFovLH(fov, aspect_ratio, depth.x, depth.y);
}
