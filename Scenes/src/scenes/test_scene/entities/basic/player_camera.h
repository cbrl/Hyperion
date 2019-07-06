#pragma once

#include "scene/components/transform/transform.h"
#include "scene/components/camera/perspective_camera.h"
#include "components/motor/camera_movement.h"
#include "components/motor/mouse_rotation.h"


namespace EntityTemplates {

struct PlayerCamera final {
	void operator()(ecs::ECS& ecs, handle64 entity, ID3D11Device& device, u32_2 viewport_size) {
		ecs.add<Transform>(entity);
		ecs.add<PerspectiveCamera>(entity, device, viewport_size);
		ecs.add<CameraMovement>(entity);
		ecs.add<MouseRotation>(entity);
	}
};

} // namespace EntityTemplates

