#pragma once

#include "world_object.h"
#include "resource\model\model.h"


class BasicModel : public WorldObject<BasicModel> {
	public:
		BasicModel(ID3D11Device* device, shared_ptr<ModelBlueprint> blueprint) {
			this->AddComponent<Model>(device, blueprint);
		}

		~BasicModel() = default;
};