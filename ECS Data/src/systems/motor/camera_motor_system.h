#pragma once

#include "system/system.h"
#include "events/gui_focus_event.h"
#include "components/motor/camera_movement.h"
#include "scene/components/transform/transform.h"


class CameraMotorSystem final : public System<CameraMotorSystem>, public EventListener {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraMotorSystem() = default;
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
	void update(Engine& engine) override;

	void registerCallbacks() override;

	void processInput(const Engine& engine, CameraMovement& movement, Transform& transform) const;
	void updateMovement(CameraMovement& mv, f32_3 units) const;
	void move(CameraMovement& mv, Transform& transform) const;
	void decelerate(CameraMovement& mv) const;

private:

	void onGuiFocus(const GuiFocusEvent& event);
};