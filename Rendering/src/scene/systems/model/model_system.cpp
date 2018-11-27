#include "model_system.h"

#include "engine/engine.h"
#include "scene/components/transform/transform.h"
#include "scene/components/model/model.h"


void ModelSystem::update(Engine& engine) {
	auto& scene          = engine.getScene();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	scene.forEach<ModelRoot>([&](ModelRoot& root) {

		if (!root.isActive()) return;

		if (const auto* transform = root.getOwner()->getComponent<Transform>()) {

			// Update the model's buffer
			root.updateBuffers(device_context, transform->getObjectToWorldMatrix());
		}
	});
}
