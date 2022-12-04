module;

#include "memory/handle/handle.h"
#include "rendering_forward_decs.h"

export module systems.camera_system;

import ecs;

import events.core_events;
import components.camera.perspective_camera;
import components.camera.orthographic_camera;
import components.transform;

import rendering;


namespace render::systems {

export class CameraSystem final : public ecs::System {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraSystem(ecs::ECS& ecs, const RenderingMgr& rendering_mgr)
		: System(ecs)
		, rendering_mgr(rendering_mgr)
		, window_resize_connection(ecs.getDispatcher<events::WindowResizeEvent>().addCallback<&CameraSystem::onWindowResize>(this)) {
	}

	CameraSystem(const CameraSystem&) = delete;
	CameraSystem(CameraSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~CameraSystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	CameraSystem& operator=(const CameraSystem&) = delete;
	CameraSystem& operator=(CameraSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override {
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

private:

	void onWindowResize(const events::WindowResizeEvent& event) {
		getECS().forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
			camera.getViewport().setSize(event.new_size);
		});
		getECS().forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
			camera.getViewport().setSize(event.new_size);
		});
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<const RenderingMgr> rendering_mgr;
	ecs::UniqueDispatcherConnection window_resize_connection;
};

} //namespace render::systems
