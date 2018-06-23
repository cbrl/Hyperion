#pragma once

#include "system/system.h"
#include "components/transform/transform.h"
#include "components/motor/camera_movement.h"


class CameraMotorSystem final : public System<CameraMotorSystem> {
public:
	CameraMotorSystem() = default;
	~CameraMotorSystem() = default;

	void update(const Engine& engine) override;
	void processInput(const Engine& engine, CameraMovement* movement, Transform* transform) const;
	void updateMovement(CameraMovement* mv, float3 units) const;
	void move(CameraMovement* mv, Transform* transform, float dt) const;
	void decelerate(CameraMovement* mv, float delta_time) const;
};