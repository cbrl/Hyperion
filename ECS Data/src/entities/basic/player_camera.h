#pragma once

#include "scene/components/transform/transform.h"
#include "scene/components/camera/perspective_camera.h"
#include "components/motor/camera_movement.h"
#include "components/motor/mouse_rotation.h"


namespace EntityTemplates {

struct PlayerCamera final {
	static void applyTemplate(Entity& entity, ID3D11Device& device, vec2_u32 viewport_size) {
		entity.addComponent<Transform>();
		entity.addComponent<PerspectiveCamera>(device, viewport_size);
		entity.addComponent<CameraMovement>();
		entity.addComponent<MouseRotation>();
	}
};

} // namespace EntityTemplates

