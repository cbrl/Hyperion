#include "camera_system.h"

#include "engine/engine.h"
#include "scene/components/camera/perspective_camera.h"
#include "scene/components/camera/orthographic_camera.h"


void CameraSystem::update(Engine& engine) {
	auto& scene          = engine.getScene();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	const auto process_cam = [&](auto& camera) {
		const auto* transform = camera.getOwner()->getComponent<Transform>();
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


void CameraSystem::registerCallbacks() {
	registerEventCallback(&CameraSystem::onWindowResize);
}


void CameraSystem::onWindowResize(const WindowResizeEvent& event) {
	getECS().forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
		camera.getViewport().setSize(event.new_size);
	});
	getECS().forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
		camera.getViewport().setSize(event.new_size);
	});
}