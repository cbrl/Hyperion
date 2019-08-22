#include "camera_system.h"

#include "engine/engine.h"
#include "scene/components/camera/perspective_camera.h"
#include "scene/components/camera/orthographic_camera.h"


namespace render::systems {

CameraSystem::CameraSystem(ecs::ECS& ecs, const RenderingMgr& rendering_mgr)
	: System(ecs)
	, rendering_mgr(rendering_mgr)
	, window_resize_connection(ecs.getDispatcher<events::WindowResizeEvent>().addCallback<&CameraSystem::onWindowResize>(this)) {
}


void CameraSystem::update() {
	auto& ecs            = this->getECS();
	auto& device_context = rendering_mgr.get().getDeviceContext();

	ecs.forEach<Transform, PerspectiveCamera>([&](handle64 entity) {
		const auto& transform = ecs.get<Transform>(entity);
		const auto& camera    = ecs.get<PerspectiveCamera>(entity);

		if (camera.isActive()) {
			camera.updateBuffer(device_context,
				                transform.getObjectToWorldMatrix(),
				                transform.getWorldToObjectMatrix());
		}
	});

	ecs.forEach<Transform, OrthographicCamera>([&](handle64 entity) {
		const auto& transform = ecs.get<Transform>(entity);
		const auto& camera    = ecs.get<OrthographicCamera>(entity);

		if (camera.isActive()) {
			camera.updateBuffer(device_context,
				                transform.getObjectToWorldMatrix(),
				                transform.getWorldToObjectMatrix());
		}
	});
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