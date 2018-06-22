#pragma once

#include "system/system.h"
#include "components/geometry/transform/transform.h"
#include "components/rendering/camera/camera_base.h"
#include "components/rendering/camera/camera_movement.h"


class CameraSystem final : public System<CameraSystem> {
public:
	CameraSystem() = default;
	~CameraSystem() = default;

	void update(const Engine& engine) override;


private:
	void processMovement(const Engine& engine, CameraMovement* movement, Transform* transform) const;
	void updateMovement(CameraMovement* mv, float3 units) const;
	void move(CameraMovement* mv, Transform* transform, float dt) const;
	void decelerate(CameraMovement* mv, float delta_time) const;
};
