#include "camera_system.h"
#include "engine/engine.h"
#include "components/camera/perspective_camera.h"
#include "components/camera/orthographic_camera.h"


void CameraSystem::update(Engine& engine) {
	auto& scene          = engine.getScene();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	const auto process_cam = [&](auto& camera) {
		const auto* transform = camera.getOwner()->template getComponent<Transform>();
		if (!transform) return;

		// Update the camera's buffer
		camera.updateBuffer(device_context,
		                    transform->getObjectToWorldMatrix(),
		                    transform->getWorldToObjectMatrix());
	};

	scene.forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
		if (camera.isActive())
			process_cam(camera);
	});


	scene.forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
		if (camera.isActive())
			process_cam(camera);
	});
}
