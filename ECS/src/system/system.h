#pragma once

#include "datatypes/datatypes.h"
#include "event/event_participator.h"

class ECS;


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
	    : ecs(nullptr)
	    , active(true)
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
		if (!state)
			time_since_last_update = 0.0;
	}

	// Get the system's state
	[[nodiscard]]
	bool isActive() const noexcept {
		return active;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Update Interval
	//----------------------------------------------------------------------------------

	// Set the time that must pass between updates for this system. This method can be
	// overridden in order to disallow changing the update interval.
	virtual void setUpdateInterval(f32 seconds) noexcept {
		update_interval = seconds;
	}

	// Get the time that must pass between updates for this system.
	[[nodiscard]]
	f32 getUpdateInterval() const noexcept {
		return update_interval;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Update
	//----------------------------------------------------------------------------------

	// Actions taken before any systems have executed their main update
	virtual void preUpdate() = 0;

	// Main update function
	virtual void update() = 0;

	// Actions taken after all systems have executed their main update
	virtual void postUpdate() = 0;

protected:

	// Retrieve the total time passed since the last update.
	// Inactive systems do not accumulate time.
	[[nodiscard]]
	f64 dtSinceLastUpdate() const noexcept {
		return time_since_last_update;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - ECS
	//----------------------------------------------------------------------------------

	// Get a reference to the ECS this system is a part of. The reference is assigned
	// after construction, so this function cannot be called in a system's constructor.
	[[nodiscard]]
	ECS& getECS() {
		assert(ecs != nullptr && "ISystem::ecs == nullptr (Was getECS() called in a system's constructor?)");
		return *ecs;
	}

private:

	void setECS(gsl::not_null<ECS*> pointer) {
		ecs = pointer;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A pointer to the ECS that created the system.
	ECS* ecs;

	// Is this system enabled?
	bool active;

	// The amount of time between that the system should wait before updating (in seconds).
	// 0.0f or lower to update every tick.
	f32 update_interval;

	// The time passed since the last update and a flag set when that time exceeds the update
	// interval. These variables are managed by the System Manager. This variable is not
	// updated if the system is inactive.
	f64  time_since_last_update;
	bool needs_update;
};




//----------------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------------
//
// Base class that all unique systems will derive from. Systems have a reference
// to the ECS that created them, allowing access to ECS functions by default.
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
	void preUpdate() override {};

	// Main update function
	void update() override{};

	// Actions taken after all systems have executed their main update
	void postUpdate() override{};

public:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// An ID unique to type T
	static const std::type_index index;
};


template <typename T>
const std::type_index System<T>::index = std::type_index(typeid(T));
