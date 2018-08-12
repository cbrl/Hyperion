#pragma once

#include "system/system.h"


class MouseRotationSystem final : public System<MouseRotationSystem> {
public:
	MouseRotationSystem() = default;
	~MouseRotationSystem() = default;

	void update(Engine& engine) override;
};