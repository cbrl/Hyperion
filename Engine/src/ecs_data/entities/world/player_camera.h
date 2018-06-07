#pragma once

#include "entity\entity.h"
#include "ecs_data\components\geometry\transform\camera_transform.h"
#include "ecs_data\components\rendering\camera\perspective_camera.h"
#include "ecs_data\components\rendering\camera\camera_movement.h"
#include "resource\resource_mgr.h"


class PlayerCamera final : public Entity<PlayerCamera> {
	public:
		PlayerCamera(Handle64 this_handle,
		             ComponentMgr* component_mgr,
		             ID3D11Device& device,
		             ID3D11DeviceContext& device_context,
		             ResourceMgr& resource_mgr,
		             u32 viewport_width,
		             u32 viewport_height,
		             const wstring& skybox_texture)
			: Entity(this_handle, component_mgr) {

				this->AddComponent<PerspectiveCamera>(device, device_context, viewport_width, viewport_height);
				this->AddComponent<CameraTransform>();
				this->AddComponent<CameraMovement>();
				this->AddComponent<SkyBox>(device, resource_mgr, skybox_texture);
		}

		PlayerCamera(Handle64 this_handle,
		             ComponentMgr* component_mgr,
		             ID3D11Device& device,
		             ID3D11DeviceContext& device_context,
		             u32 viewport_width,
		             u32 viewport_height,
		             shared_ptr<Texture> skybox_texture)
			: Entity(this_handle, component_mgr) {

			this->AddComponent<PerspectiveCamera>(device, device_context, viewport_width, viewport_height);
			this->AddComponent<CameraTransform>();
			this->AddComponent<CameraMovement>();
			this->AddComponent<SkyBox>(device, skybox_texture);
		}

		~PlayerCamera() = default;
};