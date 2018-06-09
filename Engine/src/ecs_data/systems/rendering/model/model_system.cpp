#include "stdafx.h"
#include "model_system.h"
#include "engine/engine.h"
#include "ecs_data/components/geometry/transform/transform.h"
#include "ecs_data/components/rendering/model/model.h"


void ModelSystem::update(const Engine& engine) {
	auto& ecs_engine = engine.getECS();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	ecs_engine.forEachActive<Model>([&](Model& model) {

		const auto owner = model.getOwner();
		const auto transform = ecs_engine.getComponent<Transform>(owner);

		if (transform) {
			if (transform->isUpdated()) {
				// Update the model's bounding volumes
				model.UpdateBoundingVolumes(transform->getObjectToWorldMatrix());
			}

			// Update the model's buffer
			model.updateBuffer(device_context, transform->getObjectToWorldMatrix());
		}
	});
}
