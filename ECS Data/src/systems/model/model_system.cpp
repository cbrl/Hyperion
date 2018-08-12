#include "model_system.h"
#include "engine/engine.h"
#include "components/transform/transform.h"
#include "components/model/model.h"


void ModelSystem::update(Engine& engine) {
	auto& ecs_engine     = engine.getScene().getECS();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	ecs_engine.forEach<Model>([&](Model& model) {

		if (!model.isActive()) return;

		auto* entity = ecs_engine.getEntity(model.getOwner());

		if (const auto* transform = entity->getComponent<Transform>()) {

			// Update the model's buffer
			model.updateBuffer(device_context, transform->getObjectToWorldMatrix());
		}
	});
}
