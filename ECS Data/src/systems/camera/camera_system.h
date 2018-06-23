#pragma once

#include "system/system.h"


class CameraSystem final : public System<CameraSystem> {
public:
	CameraSystem() = default;
	~CameraSystem() = default;

	void update(const Engine& engine) override;
};
