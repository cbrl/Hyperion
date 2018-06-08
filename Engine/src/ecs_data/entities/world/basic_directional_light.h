#pragma once

#include "ecs_data\entities\world\world_object.h"
#include "ecs_data\components\rendering\light\directional_light.h"


class BasicDirectionalLight final : public WorldObject<BasicDirectionalLight> {
	public:
		BasicDirectionalLight(handle64 this_handle, ComponentMgr* component_mgr)
			: WorldObject(this_handle, component_mgr) {

			this->addComponent<Transform>();
			this->addComponent<DirectionalLight>();
		}

		~BasicDirectionalLight() = default;
};