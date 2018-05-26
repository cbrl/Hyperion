#pragma once

#include "system\system.h"


class ModelSystem final : public System<ModelSystem> {
	public:
		ModelSystem() = default;
		~ModelSystem() = default;

		void Update(const Engine& engine) override;
};