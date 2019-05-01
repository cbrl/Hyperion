#pragma once

#include "event/event.h"
#include "datatypes/datatypes.h"


//----------------------------------------------------------------------------------
// Transform
//----------------------------------------------------------------------------------

class Transform;

struct TransformNeedsUpdate : public ecs::Event<TransformNeedsUpdate> {
	TransformNeedsUpdate(Transform& transform)
	    : transform(transform) {
	}

	const std::reference_wrapper<Transform> transform;
};

struct TransformUpdated : public ecs::Event<TransformUpdated> {
	TransformUpdated(Transform& transform)
	    : transform(transform) {
	}

	const std::reference_wrapper<Transform> transform;
};




//----------------------------------------------------------------------------------
// Resize
//----------------------------------------------------------------------------------
struct WindowResizeEvent : public ecs::Event<WindowResizeEvent> {
	WindowResizeEvent(u32_2 size)
	    : new_size(std::move(size)) {
	}

	const u32_2 new_size;
};




//----------------------------------------------------------------------------------
// UI Focus
//----------------------------------------------------------------------------------
struct GuiFocusEvent : public ecs::Event<GuiFocusEvent> {
	GuiFocusEvent(bool keyboard, bool mouse)
	    : keyboard_focus(keyboard)
	    , mouse_focus(mouse) {
	}

	const bool keyboard_focus;
	const bool mouse_focus;
};