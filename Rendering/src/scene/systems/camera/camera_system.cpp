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

	getECS().forEach<Transform, PerspectiveCamera>([&](ecs::Entity& entity) {
		const auto& transform = entity.get<Transform>();
		const auto& camera    = entity.get<PerspectiveCamera>();

		if (camera.isActive()) {
			camera.updateBuffer(device_context,
				                transform.getObjectToWorldMatrix(),
				                transform.getWorldToObjectMatrix());
		}
	});

	getECS().forEach<Transform, OrthographicCamera>([&](ecs::Entity& entity) {
		const auto& transform = entity.get<Transform>();
		const auto& camera    = entity.get<OrthographicCamera>();

		if (camera.isActive()) {
			camera.updateBuffer(device_context,
				                transform.getObjectToWorldMatrix(),
				                transform.getWorldToObjectMatrix());
		}
	});
}


void CameraSystem::registerCallbacks() {
	registerEventCallback(&CameraSystem::onWindowResize);
}


void CameraSystem::onWindowResize(const events::WindowResizeEvent& event) {
	getECS().forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
		camera.getViewport().setSize(event.new_size);
	});
	getECS().forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
		camera.getViewport().setSize(event.new_size);
	});
}

} //namespace systems