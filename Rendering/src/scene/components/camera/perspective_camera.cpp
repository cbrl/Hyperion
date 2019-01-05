#include "perspective_camera.h"


PerspectiveCamera::PerspectiveCamera(ID3D11Device& device, vec2_u32 viewport_size)
	: CameraBase(device)
	, fov(XM_PI / 4.0f) {

	viewport.setSize(viewport_size);
}

void PerspectiveCamera::setFOV(f32 radians) noexcept {
	fov = radians;
}

f32 PerspectiveCamera::getFOV() const noexcept {
	return fov;
}

XMMATRIX XM_CALLCONV PerspectiveCamera::getCameraToProjectionMatrix() const {

	const vec2_u32 size    = viewport.getSize();
	const f32 aspect_ratio = static_cast<f32>(size[0]) / static_cast<f32>(size[1]);

	return XMMatrixPerspectiveFovLH(fov, aspect_ratio, depth[0], depth[1]);
}
