module;

#include <functional>

#include "memory/handle/handle.h"
#include "datatypes/types.h"

export module events.core_events;


export namespace render::events {

//----------------------------------------------------------------------------------
// Resize
//----------------------------------------------------------------------------------
struct WindowResizeEvent {
	WindowResizeEvent(u32_2 size) : new_size(std::move(size)) {
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
	EntitySelectedEvent(handle64 entity) : entity(entity) {
	}

	handle64 entity;
};

} //namespace render::events
