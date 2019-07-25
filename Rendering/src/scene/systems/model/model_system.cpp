#include "model_system.h"

#include "engine/engine.h"
#include "scene/components/transform/transform.h"
#include "scene/components/model/model.h"


namespace render::systems {

ModelSystem::ModelSystem(ecs::ECS& ecs, const RenderingMgr& rendering_mgr)
	: System(ecs)
	, rendering_mgr(rendering_mgr) {
}


void ModelSystem::update() {
	auto& ecs            = this->getECS();
	auto& device_context = rendering_mgr.get().getDeviceContext();

	ecs.forEach<Transform, Model>([&](handle64 entity) {
		const auto& transform = ecs.get<Transform>(entity);
		auto& model           = ecs.get<Model>(entity);

		// Update the model's buffer
		if (model.isActive()) {
			model.updateBuffer(device_context, transform.getObjectToWorldMatrix());
		}
	});
}

} //namespace systems