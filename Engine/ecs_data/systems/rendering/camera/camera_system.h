#pragma once

#include "ecs\ecs.h"
#include "ecs_data\components\geometry\transform\camera_transform.h"
#include "ecs_data\components\rendering\camera\camera_base.h"
#include "ecs_data\components\rendering\camera\camera_movement.h"


class CameraSystem final : public System<CameraSystem> {
	public:
		CameraSystem() = default;
		~CameraSystem() = default;

		void Update(float dt) override;


	private:
		void ProcessMovement(CameraMovement* movement, CameraTransform* transform, float dt);
		void UpdateMovement(CameraMovement* mv, float3 units);
		void Move(CameraMovement* mv, CameraTransform* transform, float dt);
		void Decelerate(CameraMovement* mv, float delta_time);
};