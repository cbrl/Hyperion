#pragma once

#include "entities/world_object.h"
#include "scene/components/camera/perspective_camera.h"
#include "components/motor/camera_movement.h"
#include "components/motor/mouse_rotation.h"


class PlayerCamera final : public WorldObject<PlayerCamera> {
public:
	PlayerCamera(EntityPtr this_ptr,
	             ComponentMgr* component_mgr,
	             ID3D11Device& device,
				 vec2_u32 viewport_size)
		: WorldObject(this_ptr, component_mgr) {

		this->addComponent<PerspectiveCamera>(device, viewport_size);
		this->addComponent<CameraMovement>();
		this->addComponent<MouseRotation>();
	}

	~PlayerCamera() = default;
};
