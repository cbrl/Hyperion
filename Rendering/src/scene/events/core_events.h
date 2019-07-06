#pragma once

#include "event/event.h"
#include "memory/handle/handle.h"
#include "datatypes/datatypes.h"

class Transform;

namespace render::events {

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


//----------------------------------------------------------------------------------
// Entity Selected
//----------------------------------------------------------------------------------
struct EntitySelectedEvent : public ecs::Event<EntitySelectedEvent> {
	EntitySelectedEvent(handle64 entity)
		: entity(std::move(entity)) {
	}

	const handle64 entity;
};

} //namespace render::events