#pragma once

#include "system/system.h"


class AxisRotationSystem final : public System<AxisRotationSystem> {
public:
	AxisRotationSystem() = default;
	~AxisRotationSystem() = default;

	void update(const Engine& engine) override;
};