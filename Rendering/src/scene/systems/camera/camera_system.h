#pragma once

#include "system/system.h"
#include "event/event_dispatcher.h"
#include "scene/events/core_events.h"


namespace render {

class RenderingMgr;

namespace systems {

class CameraSystem final : public ecs::System<CameraSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraSystem(ecs::ECS& ecs, const RenderingMgr& rendering_mgr);
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
	void update() override;

private:

	void onWindowResize(const events::WindowResizeEvent& event);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<const RenderingMgr> rendering_mgr;
	ecs::UniqueDispatcherConnection window_resize_connection;
};

} //namespace systems
} //namespace render