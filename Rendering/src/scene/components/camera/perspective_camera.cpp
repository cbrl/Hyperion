#include "perspective_camera.h"


PerspectiveCamera::PerspectiveCamera(ID3D11Device& device, u32_2 viewport_size)
	: CameraBase(device) {

	viewport.setSize(viewport_size);
}

void PerspectiveCamera::setFOV(f32 radians) noexcept {
	fov = radians;
}

f32 PerspectiveCamera::getFOV() const noexcept {
	return fov;
}

XMMATRIX XM_CALLCONV PerspectiveCamera::getCameraToProjectionMatrix() const {
	const f32 aspect_ratio = viewport.getAspectRatio();
	return XMMatrixPerspectiveFovLH(fov, aspect_ratio, depth[0], depth[1]);
}
