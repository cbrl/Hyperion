#include "model_system.h"

#include "engine/engine.h"
#include "scene/components/transform/transform.h"
#include "scene/components/model/model.h"


ModelSystem::ModelSystem(const RenderingMgr& rendering_mgr)
	: rendering_mgr(rendering_mgr) {
}


void ModelSystem::update() {
	auto& device_context = rendering_mgr.getDeviceContext();

	getECS().forEach<Model>([&](Model& model) {

		if (!model.isActive()) return;

		if (const auto* transform = model.getOwner()->getComponent<Transform>()) {

			// Update the model's buffer
			model.updateBuffer(device_context, transform->getObjectToWorldMatrix());
		}
	});
}
