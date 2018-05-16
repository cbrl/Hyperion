#pragma once

#include "ecs\entity\entity.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\rendering\light\spot_light.h"


class BasicSpotLight final : public Entity<BasicSpotLight> {
	public:
		BasicSpotLight() = default;
		~BasicSpotLight() = default;

		void Init() {
			this->AddComponent<Transform>();
			this->AddComponent<SpotLight>();
		}
};