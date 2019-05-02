#include "perspective_camera.h"


namespace render {

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

	const u32_2 size         = viewport.getSize();
	const f32   aspect_ratio = static_cast<f32>(size[0]) / static_cast<f32>(size[1]);

	return XMMatrixPerspectiveFovLH(fov, aspect_ratio, depth[0], depth[1]);
}

} //namespace render