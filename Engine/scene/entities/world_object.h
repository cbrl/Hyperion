#pragma once

#include "ecs\entity\entity.h"
#include "geometry\transform\transform.h"


//----------------------------------------------------------------------------------
// WorldObjecct
//----------------------------------------------------------------------------------
//
// The base class that any object with a position in the world
// will inherit from. Automatically adds a Transform component.
//
//----------------------------------------------------------------------------------

template<typename T>
class WorldObject : public Entity<T> {
	public:
		WorldObject() {
			this->AddComponent<Transform>();
		}

		virtual ~WorldObject() = default;
};