#pragma once

#include "entities/world/world_object.h"
#include "components/rendering/light/spot_light.h"


class BasicSpotLight final : public WorldObject<BasicSpotLight> {
public:
	BasicSpotLight(handle64 this_handle, ComponentMgr* component_mgr)
		: WorldObject(this_handle, component_mgr) {

		this->addComponent<Transform>();
		this->addComponent<SpotLight>();
	}

	~BasicSpotLight() = default;
};