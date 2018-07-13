#include "perspective_camera.h"


PerspectiveCamera::PerspectiveCamera(ID3D11Device& device,
                                     u32 viewport_width,
                                     u32 viewport_height)
	: CameraBase(device)
	, fov(XM_PI / 4.0f) {

	viewport.setSize(viewport_width, viewport_height);
}


XMMATRIX XM_CALLCONV PerspectiveCamera::getCameraToProjectionMatrix() const {

	const vec2_u32 size    = viewport.getSize();
	const f32 aspect_ratio = static_cast<f32>(size.x) / static_cast<f32>(size.y);

	return XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);
}
