#pragma once

#include "system/system.h"
#include "events/gui_focus_event.h"
#include "components/motor/camera_movement.h"
#include "scene/components/transform/transform.h"

class Input;

class CameraMotorSystem final : public System<CameraMotorSystem>, public EventListener {
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

	void onGuiFocus(const GuiFocusEvent& event);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	const Input& input;
};