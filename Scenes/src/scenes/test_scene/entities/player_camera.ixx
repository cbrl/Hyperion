module;

#include "datatypes/scalar_types.h"
#include "memory/handle/handle.h"

#include "directx/d3d11.h"

export module entities.player_camera;

import ecs;
import components.motor.camera_movement;
import components.motor.mouse_rotation;
import components.camera.perspective_camera;
import components.transform;


export namespace EntityTemplates {

	struct PlayerCamera final {
		void operator()(ecs::ECS& ecs, handle64 entity, ID3D11Device& device, u32_2 viewport_size) {
			ecs.add<Transform>(entity);
			ecs.add<PerspectiveCamera>(entity, device, viewport_size);
			ecs.add<CameraMovement>(entity);
			ecs.add<MouseRotation>(entity);
		}
	};

} // namespace EntityTemplates
