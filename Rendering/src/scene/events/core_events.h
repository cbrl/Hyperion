#pragma once

#include "event/event.h"
#include "datatypes/datatypes.h"


//----------------------------------------------------------------------------------
// Transform
//----------------------------------------------------------------------------------

class Transform;

struct TransformNeedsUpdate : public Event<TransformNeedsUpdate> {
	TransformNeedsUpdate(Transform& transform)
	    : transform(transform) {}
	std::reference_wrapper<Transform> transform;
};

struct TransformUpdated : public Event<TransformUpdated> {
	TransformUpdated(Transform& transform)
	    : transform(transform) {}
	std::reference_wrapper<Transform> transform;
};




//----------------------------------------------------------------------------------
// Resize
//----------------------------------------------------------------------------------

struct WindowResizeEvent : public Event<WindowResizeEvent> {
	WindowResizeEvent(vec2_u32 size)
	    : new_size(std::move(size)) {}
	vec2_u32 new_size;
};