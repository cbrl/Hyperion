#include "model_system.h"

#include "engine/engine.h"
#include "scene/components/transform/transform.h"
#include "scene/components/model/model.h"


namespace render::systems {

ModelSystem::ModelSystem(const RenderingMgr& rendering_mgr)
	: rendering_mgr(rendering_mgr) {
}


void ModelSystem::update() {
	auto& device_context = rendering_mgr.getDeviceContext();

	getECS().forEach<Transform, Model>([&](ecs::Entity& entity) {
		const auto& transform = *entity.getComponent<Transform>();
		const auto  models    = entity.getAll<Model>();

		for (Model& model : models) {
			if (!model.isActive())
				continue;

			// Update the model's buffer
			model.updateBuffer(device_context, transform.getObjectToWorldMatrix());
		}
	});
}

} //namespace systems