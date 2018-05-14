#pragma once

#include "ecs\system\system.h"
#include "ecs_data\components\geometry\transform\camera_transform.h"
#include "ecs_data\components\rendering\camera\camera_base.h"
#include "ecs_data\components\rendering\camera\camera_movement.h"


class CameraSystem final : public System<CameraSystem> {
	public:
		CameraSystem() = default;
		~CameraSystem() = default;

		void Update(const Engine& engine) override;


	private:
		void ProcessMovement(const Engine& engine, CameraMovement* movement, CameraTransform* transform);
		void UpdateMovement(CameraMovement* mv, float3 units);
		void Move(CameraMovement* mv, CameraTransform* transform, float dt);
		void Decelerate(CameraMovement* mv, float delta_time);
};