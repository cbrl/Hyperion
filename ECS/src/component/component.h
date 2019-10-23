#pragma once

#include "datatypes/types.h"
#include "memory/handle/handle.h"


namespace ecs {

//----------------------------------------------------------------------------------
// IComponent
//----------------------------------------------------------------------------------
//
// Interface for the Component class
//
//----------------------------------------------------------------------------------

class IComponent {
	friend class ComponentMgr;

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
protected:

	IComponent() noexcept = default;

public:

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

	// Get the component's type index
	[[nodiscard]]
	virtual const std::type_index& getTypeIndex() const = 0;

	// Get the ptr of the entity that owns this component
	[[nodiscard]]
	handle64 getOwner() const noexcept {
		return owner;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - State
	//----------------------------------------------------------------------------------

	// Set the component's state
	void setActive(bool state) {
		active = state;
	}

	// Get the component's state
	[[nodiscard]]
	bool isActive() const {
		return active;
	}

private:

	void setOwner(handle64 owner_handle) {
		owner = owner_handle;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Is the component active?
	bool active = true;

	// The entity that owns this component. Set on creation in IEntity.
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
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
protected:

	Component() noexcept {
		// Assert in the constructor since the type isn't fully defined before here
		static_assert(std::is_move_constructible_v<T>,
			"Component type is not move constructible");

		static_assert(std::is_move_assignable_v<T>,
			"Component type is not move assignible");
	}

public:

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
	[[nodiscard]]
	const std::type_index& getTypeIndex() const override final {
		return index;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// An ID unique to type T
	static const std::type_index index;
};


template<typename T>
const std::type_index Component<T>::index = std::type_index{typeid(T)};

} // namespace ecs