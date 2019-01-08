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
	    : active(true)
		, update_interval(-1.0f)
	    , time_since_last_update(FLT_MAX)
		, needs_update(true) {
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
	[[nodiscard]]
	virtual std::type_index getTypeIndex() const = 0;


	//----------------------------------------------------------------------------------
	// Member Functions - State
	//----------------------------------------------------------------------------------

	// Set the system's state
	void setActive(bool state) noexcept {
		active = state;
	}

	// Get the system's state
	[[nodiscard]]
	bool isActive() const noexcept {
		return active;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Update Interval
	//----------------------------------------------------------------------------------

	void setUpdateInterval(f32 seconds) noexcept {
		update_interval = seconds;
	}

	[[nodiscard]]
	f32 getUpdateInterval() const noexcept {
		return update_interval;
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


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A pointer to the ECS that created the system. Useful for iterating over
	// components while processing an event.
	ECS* ecs;

	// Is this system enabled?
	bool active;

	// The amount of time between that the system should wait before updating (in seconds).
	// 0.0f or lower to update every tick.
	f32 update_interval;

	f32  time_since_last_update;
	bool needs_update;
};




//----------------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------------
//
// Base class that all unique systems will derive from
//
//----------------------------------------------------------------------------------

template <typename T>
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
	[[nodiscard]]
	std::type_index getTypeIndex() const override final {
		return index;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------

	// Actions taken before any systems have executed their main update
	void preUpdate(Engine& engine) override{};

	// Main update function
	void update(Engine& engine) override{};

	// Actions taken after all systems have executed their main update
	void postUpdate(Engine& engine) override{};


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// An ID unique to type T
	static const std::type_index index;
};


template <typename T>
const std::type_index System<T>::index = std::type_index(typeid(T));
