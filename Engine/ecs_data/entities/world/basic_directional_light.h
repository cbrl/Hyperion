#pragma once

#include "ecs\entity\entity.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\rendering\light\directional_light.h"


class BasicDirectionalLight final : public Entity<BasicDirectionalLight> {
	public:
		BasicDirectionalLight() = default;
		~BasicDirectionalLight() = default;

		void Init() {
			this->AddComponent<Transform>();
			this->AddComponent<DirectionalLight>();
		}
};