#pragma once

#include "entity/entity.h"
#include "scene/components/transform/transform.h"


//----------------------------------------------------------------------------------
// WorldObjecct
//----------------------------------------------------------------------------------
//
// The base class that any object with a position in the world
// will inherit from. Automatically adds a Transform component.
//
//----------------------------------------------------------------------------------

template<typename T = void>
class WorldObject : public Entity<WorldObject<T>> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	WorldObject(EntityPtr this_ptr, ComponentMgr* component_mgr)
		: Entity<WorldObject<T>>(this_ptr, component_mgr) {

		this->addComponent<Transform>();
	}

	WorldObject(const WorldObject& obj) = delete;
	WorldObject(WorldObject&& obj) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~WorldObject() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	WorldObject& operator=(const WorldObject& obj) = delete;
	WorldObject& operator=(WorldObject&& obj) noexcept = default;
};
