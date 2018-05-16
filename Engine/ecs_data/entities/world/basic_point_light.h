#pragma once

#include "ecs\entity\entity.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\rendering\light\point_light.h"

 
class BasicPointLight final : public Entity<BasicPointLight> {
	public:
		BasicPointLight() = default;
		~BasicPointLight() = default;

		void Init() {
			this->AddComponent<Transform>();
			this->AddComponent<PointLight>();
		}
};