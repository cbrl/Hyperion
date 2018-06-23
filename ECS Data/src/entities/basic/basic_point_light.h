#pragma once

#include "entities/world_object.h"
#include "components/light/point_light.h"


class BasicPointLight final : public WorldObject<BasicPointLight> {
public:
	BasicPointLight(handle64 this_handle, ComponentMgr* component_mgr)
		: WorldObject(this_handle, component_mgr) {

		this->addComponent<Transform>();
		this->addComponent<PointLight>();
	}

	~BasicPointLight() = default;
};
