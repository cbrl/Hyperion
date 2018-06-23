#pragma once

#include "entities/world_object.h"
#include "components/camera/perspective_camera.h"
#include "components/motor/camera_movement.h"
#include "components/motor/mouse_rotation.h"


class PlayerCamera final : public WorldObject<PlayerCamera> {
public:
	PlayerCamera(handle64 this_handle,
	             ComponentMgr* component_mgr,
	             ID3D11Device& device,
	             ID3D11DeviceContext& device_context,
	             u32 viewport_width,
	             u32 viewport_height)
		: WorldObject(this_handle, component_mgr) {

		this->addComponent<PerspectiveCamera>(device, device_context, viewport_width, viewport_height);
		this->addComponent<Transform>();
		this->addComponent<CameraMovement>();
		this->addComponent<MouseRotation>();
	}

	~PlayerCamera() = default;
};
