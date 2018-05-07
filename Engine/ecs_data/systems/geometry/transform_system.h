#pragma once

#include "ecs\ecs.h"
#include "ecs_data\components\geometry\transform\transform.h"


class TransformSystem final : public System<TransformSystem> {
	public:
		TransformSystem() = default;
		~TransformSystem() = default;

		void Update(float dt);
		void PostUpdate(float dt);


	private:
		void UpdateWorld(Transform& transform);
};