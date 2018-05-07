#pragma once

#include "ecs\ecs.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\rendering\camera\camera_base.h"
#include "ecs_data\components\rendering\camera\camera_movement.h"


class CameraSystem final : public System<CameraSystem> {
	public:
		CameraSystem() = default;
		~CameraSystem() = default;

		void Update(float dt) override;


	private:
		void ProcessMovement(CameraMovement* movement, Transform* transform, float dt);
		void Move(CameraMovement* mv, float3 units);
		void Rotate(CameraMovement* mv, Transform* transform, float3 units, XMVECTOR& forward, XMVECTOR& up, XMVECTOR& right);
		void UpdateMovement(CameraMovement* mv, Transform* transform, XMVECTOR& forward, XMVECTOR& up, XMVECTOR& right, float dt);
		void Decelerate(CameraMovement* mv, float delta_time);
};