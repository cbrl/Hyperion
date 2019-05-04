#pragma once

#include "system/system.h"
#include "scene/events/core_events.h"
#include "scene/components/transform/transform.h"
#include "scenes/test_scene/components/motor/camera_movement.h"

class Input;

class CameraMotorSystem final : public ecs::System<CameraMotorSystem>, public ecs::EventListener {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraMotorSystem(const Input& input);
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

	void registerCallbacks() override;

	void processInput(CameraMovement& movement, Transform& transform) const;
	void updateMovement(CameraMovement& mv, f32_3 units) const;
	void move(CameraMovement& mv, Transform& transform) const;
	void decelerate(CameraMovement& mv) const;

	void onGuiFocus(const render::events::GuiFocusEvent& event);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	const Input& input;
};