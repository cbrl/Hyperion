#include "model_system.h"
#include "engine/engine.h"
#include "components/transform/transform.h"
#include "components/model/model.h"


void ModelSystem::update(const Engine& engine) {
	auto& ecs_engine     = engine.getECS();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	ecs_engine.forEachActive<Model>([&](Model& model) {

		if (const auto transform = ecs_engine.getComponent<Transform>(model.getOwner())) {

			if (transform->isUpdated()) {
				// Update the model's buffer
				model.updateBuffer(device_context, transform->getObjectToWorldMatrix());
			}
		}
	});
}