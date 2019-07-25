#pragma once

#include "system/system.h"
#include "event/event_dispatcher.h"
#include "scene/events/core_events.h"
#include "scene/components/transform/transform.h"
#include "scenes/test_scene/components/motor/camera_movement.h"

class Input;
class KeyConfig;

class CameraMotorSystem final : public ecs::System<CameraMotorSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraMotorSystem(ecs::ECS& ecs, const Input& input, KeyConfig& key_config);
	CameraMotorSystem(const CameraMotorSystem&) = delete;
	CameraMotorSystem(CameraMotorSystem&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~CameraMotorSystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	CameraMotorSystem& operator=(const CameraMotorSystem&) = delete;
	CameraMotorSystem& operator=(CameraMotorSystem&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

private:

	void processInput(CameraMovement& movement, Transform& transform) const;
	void updateMovement(CameraMovement& mv, f32_3 units) const;
	void move(CameraMovement& mv, Transform& transform) const;
	void decelerate(CameraMovement& mv) const;

	void onGuiFocus(const render::events::GuiFocusEvent& event);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<const Input> input;
	std::reference_wrapper<KeyConfig> key_config;
	ecs::UniqueDispatcherConnection gui_focus_connection;
};