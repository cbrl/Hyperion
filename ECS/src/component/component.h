#pragma once

#include "datatypes/datatypes.h"
#include "memory/handle/handle.h"


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
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	IComponent() noexcept
		: active(true)
		, owner(handle64::invalid_handle) {
	}

	IComponent(const IComponent& component) = delete;
	IComponent(IComponent&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~IComponent() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	IComponent& operator=(const IComponent& component) = delete;
	IComponent& operator=(IComponent&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

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
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Component() noexcept = default;
	Component(const Component& component) = delete;
	Component(Component&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~Component() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Component& operator=(const Component& component) = delete;
	Component& operator=(Component&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	type_index getTypeId() const override {
		return type_id;
	}


public:
	// An ID unique to type T
	static const type_index type_id;
};


template<typename T>
const type_index Component<T>::type_id = type_index(typeid(T));
