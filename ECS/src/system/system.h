#pragma once

#include "datatypes/types.h"
#include "time/time.h"


namespace ecs {

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

	// Default system priority
	static constexpr u32 default_priority = 100;


	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ISystem(ecs::ECS& ecs) : ecs(ecs) {}
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
			time_since_last_update = 0.0s;
	}

	// Get the system's state
	[[nodiscard]]
	bool isActive() const noexcept {
		return active;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Update Interval / Priority
	//----------------------------------------------------------------------------------

	// Set the time that must pass between updates for this system. This method can be
	// overridden or made private in order to disallow changing the update interval.
	virtual void setUpdateInterval(std::chrono::duration<f64> interval) noexcept {
		update_interval = interval;
	}

	// Get the time that must pass between updates for this system.
	[[nodiscard]]
	std::chrono::duration<f64> getUpdateInterval() const noexcept {
		return update_interval;
	}

	void setPriority(u32 value) noexcept {
		priority = value;
	}

	[[nodiscard]]
	u32 getPriority() const noexcept {
		return priority;
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
	std::chrono::duration<f64> dtSinceLastUpdate() const noexcept {
		return time_since_last_update;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - ECS
	//----------------------------------------------------------------------------------

	// Get a reference to the ECS this system was created by
	[[nodiscard]]
	ECS& getECS() {
		return ecs.get();
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A reference to the ECS that created the system
	std::reference_wrapper<ECS> ecs;

	// Is this system enabled?
	bool active = true;

	// The system's priority. Higher prioity systems get updated before lower priority ones.
	u32 priority = default_priority;

	// The amount of time between that the system should wait before updating. 0 or less to update every tick.
	std::chrono::duration<f64> update_interval{-1.0f};

	// The time passed since the last update and a flag set when that time exceeds the update
	// interval. These variables are managed by the System Manager. This variable is not
	// updated if the system is inactive.
	std::chrono::duration<f64> time_since_last_update{FLT_MAX};
	bool needs_update = true;
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
	System(ecs::ECS& ecs) : ISystem(ecs) {}
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

} // namespace ecs