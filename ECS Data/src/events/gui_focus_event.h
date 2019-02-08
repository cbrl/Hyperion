#pragma once

#include "event/event.h"

struct GuiFocusEvent : public Event<GuiFocusEvent> {
	GuiFocusEvent(bool keyboard, bool mouse)
	    : keyboard_focus(keyboard)
	    , mouse_focus(mouse) {
	}

	bool keyboard_focus;
	bool mouse_focus;
};