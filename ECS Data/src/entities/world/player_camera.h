#pragma once

#include "entity/entity.h"
#include "components/geometry/transform/camera_transform.h"
#include "components/rendering/camera/perspective_camera.h"
#include "components/rendering/camera/camera_movement.h"
#include "resource/resource_mgr.h"


class PlayerCamera final : public Entity<PlayerCamera> {
public:
	PlayerCamera(handle64 this_handle,
	             ComponentMgr* component_mgr,
	             ID3D11Device& device,
	             ID3D11DeviceContext& device_context,
	             u32 viewport_width,
	             u32 viewport_height)
		: Entity(this_handle, component_mgr) {

		this->addComponent<PerspectiveCamera>(device, device_context, viewport_width, viewport_height);
		this->addComponent<CameraTransform>();
		this->addComponent<CameraMovement>();
	}

	~PlayerCamera() = default;
};
