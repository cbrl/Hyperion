#include "stdafx.h"
#include "model_system.h"
#include "engine/engine.h"
#include "ecs_data/components/geometry/transform/transform.h"
#include "ecs_data/components/rendering/model/model.h"


void ModelSystem::update(const Engine& engine) {
	auto& ecs_engine = engine.GetECS();
	auto& device_context = engine.GetRenderingMgr().GetDeviceContext();

	ecs_engine.forEachActive<Model>([&](Model& model) {

		const auto owner = model.getOwner();
		const auto transform = ecs_engine.getComponent<Transform>(owner);

		if (transform) {
			if (transform->Updated()) {
				// Update the model's bounding volumes
				model.UpdateBoundingVolumes(transform->GetObjectToWorldMatrix());
			}

			// Update the model's buffer
			model.UpdateBuffer(device_context, transform->GetObjectToWorldMatrix());
		}
	});
}
