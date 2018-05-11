#pragma once

#include "ecs\system\system.h"


class ModelSystem final : public System<ModelSystem> {
	public:
		ModelSystem() = default;
		~ModelSystem() = default;

		void Update(const Engine& engine) override;
};