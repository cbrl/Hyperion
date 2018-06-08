#pragma once

#include "datatypes/datatypes.h"
#include "handle/handle.h"


//----------------------------------------------------------------------------------
// IComponent
//----------------------------------------------------------------------------------
//
// Base class for the Component class. Only to be used by that class.
//
//----------------------------------------------------------------------------------

class IComponent {
	friend class IEntity;

public:
	IComponent()
		: active(true)
		, owner(handle64::invalid_handle) {
	}

	virtual ~IComponent() = default;

	virtual type_index getTypeId() const = 0;

	// Get the handle of the entity that owns this component
	handle64 getOwner() const {
		return owner;
	}

	void setActive(bool state) {
		active = state;
	}

	bool isActive() const {
		return active;
	}


protected:
	// Is the component active?
	bool active;

	// Set on creation in IEntity
	handle64 owner;
};


//----------------------------------------------------------------------------------
// Component
//----------------------------------------------------------------------------------
//
// Base class that all unique components will derive from
//
//----------------------------------------------------------------------------------

template<typename T>
class Component : public IComponent {
public:
	Component() = default;
	virtual ~Component() = default;

	type_index getTypeId() const override {
		return type_id;
	}


public:
	// An ID unique to type T
	static const type_index type_id;
};


template<typename T>
const type_index Component<T>::type_id = type_index(typeid(T));
