#pragma once

#include "system/system.h"
#include "components/transform/transform.h"
#include "components/motor/camera_movement.h"


class CameraMotorSystem final : public System<CameraMotorSystem> {
public:
	CameraMotorSystem() = default;
	~CameraMotorSystem() = default;

	void update(Engine& engine) override;
	void processInput(const Engine& engine, CameraMovement* movement, Transform* transform) const;
	void updateMovement(CameraMovement* mv, vec3_f32 units) const;
	void move(CameraMovement* mv, Transform* transform, f32 dt) const;
	void decelerate(CameraMovement* mv, f32 delta_time) const;
};