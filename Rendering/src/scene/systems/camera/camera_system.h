#pragma once

#include "system/system.h"
#include "scene/events/core_events.h"

class RenderingMgr;

class CameraSystem final : public ecs::System<CameraSystem>, public ecs::EventListener {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraSystem(const RenderingMgr& rendering_mgr);
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

	void registerCallbacks() override;

	void onWindowResize(const WindowResizeEvent& event);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	const RenderingMgr& rendering_mgr;
};
