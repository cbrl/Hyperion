#include "camera_system.h"
#include "engine/engine.h"
#include "components/camera/perspective_camera.h"
#include "components/camera/orthographic_camera.h"


void CameraSystem::update(const Engine& engine) {
	auto& ecs_engine     = engine.getECS();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	const auto process_cam = [&](auto& camera) {
		const auto* transform = ecs_engine.getComponent<Transform>(camera.getOwner());
		if (!transform) return;

		// Update the camera's buffer
		camera.updateBuffer(device_context,
		                    transform->getObjectToWorldMatrix(),
		                    transform->getWorldToObjectMatrix());
	};

	ecs_engine.forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
		if (camera.isActive())
			process_cam(camera);
	});


	ecs_engine.forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
		if (camera.isActive())
			process_cam(camera);
	});
}
