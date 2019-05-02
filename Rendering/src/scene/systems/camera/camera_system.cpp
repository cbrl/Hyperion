#include "camera_system.h"

#include "engine/engine.h"
#include "scene/components/camera/perspective_camera.h"
#include "scene/components/camera/orthographic_camera.h"


namespace render::systems {

CameraSystem::CameraSystem(const RenderingMgr& rendering_mgr)
	: rendering_mgr(rendering_mgr) {
}


void CameraSystem::update() {
	auto& device_context = rendering_mgr.getDeviceContext();

	const auto process_cam = [&](auto& camera) {
		const auto* transform = camera.getOwner()->getComponent<Transform>();
		if (!transform) return;

		// Update the camera's buffer
		camera.updateBuffer(device_context,
		                    transform->getObjectToWorldMatrix(),
		                    transform->getWorldToObjectMatrix());
	};

	getECS().forEach<render::PerspectiveCamera>([&](PerspectiveCamera& camera) {
		if (camera.isActive())
			process_cam(camera);
	});

	getECS().forEach<render::OrthographicCamera>([&](OrthographicCamera& camera) {
		if (camera.isActive())
			process_cam(camera);
	});
}


void CameraSystem::registerCallbacks() {
	registerEventCallback(&CameraSystem::onWindowResize);
}


void CameraSystem::onWindowResize(const events::WindowResizeEvent & event) {
	getECS().forEach<render::PerspectiveCamera>([&](PerspectiveCamera& camera) {
		camera.getViewport().setSize(event.new_size);
	});
	getECS().forEach<render::OrthographicCamera>([&](OrthographicCamera& camera) {
		camera.getViewport().setSize(event.new_size);
	});
}

} //namespace systems