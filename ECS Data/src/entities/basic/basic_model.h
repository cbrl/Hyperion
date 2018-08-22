#pragma once

#include "entities/world_object.h"
#include "components/model/model.h"


class BasicModel final : public WorldObject<BasicModel> {
public:
	BasicModel(handle64 this_handle,
	           ComponentMgr* component_mgr,
	           ID3D11Device& device,
	           shared_ptr<Mesh> mesh,
	           ModelPart& model_part,
	           Material& material)
		: WorldObject(this_handle, component_mgr) {

		this->addComponent<Model>(device, mesh, model_part, material);
	}

	~BasicModel() = default;
};
