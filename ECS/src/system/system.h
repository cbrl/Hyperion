#pragma once

#include "datatypes/datatypes.h"
#include "event/event_participator.h"

class ECS;
class Engine;


//----------------------------------------------------------------------------------
// ISystem
//----------------------------------------------------------------------------------
//
// Interface for the System class
//
//----------------------------------------------------------------------------------

class ISystem {
	friend class SystemMgr;

public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ISystem() noexcept
		: active(true) {
	}

	ISystem(const ISystem& system) = delete;
	ISystem(ISystem&& system) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~ISystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ISystem& operator=(const ISystem& system) = delete;
	ISystem& operator=(ISystem&& system) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Type Index
	//----------------------------------------------------------------------------------

	// Get the type index of this system
	virtual std::type_index getTypeIndex() const = 0;


	//----------------------------------------------------------------------------------
	// Member Functions - State
	//----------------------------------------------------------------------------------

	// Set the system's state
	void setActive(bool state) {
		active = state;
	}

	// Get the system's state
	bool isActive() const {
		return active;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------

	// Actions taken before any systems have executed their main update
	virtual void preUpdate(Engine& engine) = 0;

	// Main update function
	virtual void update(Engine& engine) = 0;

	// Actions taken after all systems have executed their main update
	virtual void postUpdate(Engine& engine) = 0;


protected:

	void setECS(gsl::not_null<ECS*> pointer) {
		ecs = pointer;
	}

	ECS& getECS() {
		assert(ecs != nullptr && "ISystem::ecs == nullptr");
		return *ecs;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Is this system enabled?
	bool active;

private:

	// A pointer to the ECS that created the system
	ECS* ecs;
};




//----------------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------------
//
// Base class that all unique systems will derive from
//
//----------------------------------------------------------------------------------

template<typename T>
class System : public ISystem {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	System() = default;
	System(const System& system) = delete;
	System(System&& system) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~System() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	System& operator=(const System& system) = delete;
	System& operator=(System&& system) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Type Index
	//----------------------------------------------------------------------------------

	// Get the type index of this system
	std::type_index getTypeIndex() const override final {
		return index;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------

	// Actions taken before any systems have executed their main update
	void preUpdate(Engine& engine) override {
	};

	// Main update function
	void update(Engine& engine) override {
	};

	// Actions taken after all systems have executed their main update
	void postUpdate( Engine& engine) override {
	};


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// An ID unique to type T
	static const std::type_index index;
};


template<typename T>
const std::type_index System<T>::index = std::type_index(typeid(T));
