#pragma once

#include "ecs\entity\entity.h"
#include "ecs_data\components\geometry\transform\camera_transform.h"
#include "ecs_data\components\rendering\camera\perspective_camera.h"
#include "ecs_data\components\rendering\camera\camera_movement.h"


class PlayerCamera final : public Entity<PlayerCamera> {
	public:
		PlayerCamera(ID3D11Device* device,
					 ID3D11DeviceContext* device_context,
					 weak_ptr<Input> input,
					 u32 viewport_width,
					 u32 viewport_height) {

			this->AddComponent<CameraTransform>();
			this->AddComponent<PerspectiveCamera>(device, device_context, viewport_width, viewport_height);
			this->AddComponent<CameraMovement>(input);
		}
};