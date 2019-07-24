#pragma once

#include "event/events.h"
#include "memory/handle/handle.h"
#include "datatypes/datatypes.h"

class Transform;

namespace render::events {

//----------------------------------------------------------------------------------
// Resize
//----------------------------------------------------------------------------------
struct WindowResizeEvent {
	WindowResizeEvent(u32_2 size)
	    : new_size(std::move(size)) {
	}

	u32_2 new_size;
};


//----------------------------------------------------------------------------------
// UI Focus
//----------------------------------------------------------------------------------
struct GuiFocusEvent {
	GuiFocusEvent(bool keyboard, bool mouse)
	    : keyboard_focus(keyboard)
	    , mouse_focus(mouse) {
	}

	bool keyboard_focus;
	bool mouse_focus;
};


//----------------------------------------------------------------------------------
// Entity Selected
//----------------------------------------------------------------------------------
struct EntitySelectedEvent {
	EntitySelectedEvent(handle64 entity)
		: entity(std::move(entity)) {
	}

	handle64 entity;
};

} //namespace render::events