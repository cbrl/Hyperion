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

class WorldObject : public Entity {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	WorldObject(EntityPtr this_ptr, ComponentMgr* component_mgr)
		: Entity(this_ptr, gsl::make_not_null(component_mgr)) {

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
