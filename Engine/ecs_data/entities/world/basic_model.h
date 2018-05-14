#pragma once

#include "world_object.h"
#include "ecs_data\components\rendering\model\model.h"


class BasicModel final : public WorldObject<BasicModel> {
	public:
		BasicModel() = default;
		~BasicModel() = default;

		void Init(ID3D11Device& device, shared_ptr<ModelBlueprint> blueprint) {
			this->AddComponent<Model>(device, blueprint);
		}
};