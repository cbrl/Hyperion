#pragma once

#include "system/system.h"
#include "scene/events/core_events.h"


class CameraSystem final : public System<CameraSystem>, public EventListener {
public:
	CameraSystem() = default;
	~CameraSystem() = default;

	void registerCallbacks() override final;
	void update(Engine& engine) override final;

private:
	void onWindowResize(const WindowResizeEvent& event);
};
