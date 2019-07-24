#pragma once

#include <typeindex>
#include <chrono>
#include "memory/handle/handle.h"

namespace ecs {

//----------------------------------------------------------------------------------
// Core Events
//----------------------------------------------------------------------------------
//
// Events sent by the ECS itself
//
//----------------------------------------------------------------------------------

// Sent just after an entity is created
struct EntityCreated {
public:
	EntityCreated(handle64 entity) : entity(entity) {}
	handle64 entity;
};

// Sent just before an entity is destroyed
struct EntityDestroyed {
public:
	EntityDestroyed(handle64 entity) : entity(entity) {}
	handle64 entity;
};

} // namespace ecs