module;

#include <algorithm>
#include <chrono>
#include <concepts>
#include <functional>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "datatypes/scalar_types.h"
#include "time/time.h"

export module ecs:system;

import exception;


template<typename T>
std::type_index get_type_index() {
	return std::type_index{typeid(T)};
}


export namespace ecs {

class ECS;
class SystemMgr;


//----------------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------------
//
// Base class that all unique systems will derive from. Systems have a reference
// to the ECS that created them, allowing access to ECS functions by default.
//
//----------------------------------------------------------------------------------
class System {
	friend class SystemMgr;

public:

	// Default system priority
	static constexpr u32 default_priority = 100;


	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	System(ECS& ecs) : ecs(ecs) {}
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
	virtual std::type_index getTypeIndex() const {
		return std::type_index{typeid(*this)};
	}


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
	virtual void preUpdate() {}

	// Main update function
	virtual void update() {}

	// Actions taken after all systems have executed their main update
	virtual void postUpdate() {}

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


class SystemMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SystemMgr() = default;

	SystemMgr(const SystemMgr& manager) = delete;
	SystemMgr(SystemMgr&& manager) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~SystemMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SystemMgr& operator=(const SystemMgr& manager) = delete;
	SystemMgr& operator=(SystemMgr&& manager) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update(std::chrono::duration<f64> dt) {
		//----------------------------------------------------------------------------------
		// Pre Update
		//----------------------------------------------------------------------------------
		for (System& system : system_queue) {

			if (system.isActive()) {
				system.time_since_last_update += dt; //update system delta time

				if (system.time_since_last_update >= system.update_interval) {
					system.needs_update = true;
				}

				if (system.needs_update) {
					system.preUpdate();
				}
			}
		}

		//----------------------------------------------------------------------------------
		// Update
		//----------------------------------------------------------------------------------
		for (System& system : system_queue) {
			if (system.isActive() and system.needs_update) {
				system.update();
			}
		}

		//----------------------------------------------------------------------------------
		// Post Update
		//----------------------------------------------------------------------------------
		for (System& system : system_queue) {
			if (system.isActive() and system.needs_update) {
				system.postUpdate();
				system.needs_update = false;
				system.time_since_last_update = 0.0s;
			}
		}
	}

	template<typename SystemT, typename... ArgsT>
	requires std::derived_from<SystemT, System> and std::constructible_from<SystemT, ArgsT...>
	SystemT& add(ArgsT&&... args) {
		const auto it = systems.find(get_type_index<SystemT>());

		// Return the system if it has already been added
		if (it != systems.end() and it->second != nullptr)
			return static_cast<SystemT&>(*(it->second));

		// Create the system
		auto  pair   = systems.try_emplace(get_type_index<SystemT>(), std::make_unique<SystemT>(std::forward<ArgsT>(args)...));
		auto& system = static_cast<SystemT&>(*(pair.first->second));

		// Add the system to the queue and sort it
		system_queue.push_back(std::ref(system));
		sortSystemQueue();

		return system;
	}

	void remove(System& system) {
		systems.erase(system.getTypeIndex());

		const auto it = std::find_if(system_queue.begin(), system_queue.end(),
			[&system](const System& current) {
				return current.getTypeIndex() == system.getTypeIndex();
			}
		);

		if (it != system_queue.end()) {
			system_queue.erase(it);
		}
	}

	template<typename SystemT>
	void remove() {
		// Remove from the array of systems
		systems.erase(get_type_index<SystemT>());

		// Remove from the sorted system queue
		const auto it = std::find_if(system_queue.begin(), system_queue.end(),
			[](const System& system) {
				return system.getTypeIndex() == get_type_index<SystemT>();
			}
		);

		if (it != system_queue.end()) {
			system_queue.erase(it);
		}
	}

	template<typename SystemT>
	[[nodiscard]]
	SystemT& get() {
		return static_cast<SystemT&>(systems.at(get_type_index<SystemT>()));
	}

	template<typename SystemT>
	[[nodiscard]]
	const SystemT& get() const {
		return static_cast<const SystemT&>(systems.at(get_type_index<SystemT>()));
	}

	template<typename SystemT>
	[[nodiscard]]
	SystemT* tryGet() {
		return const_cast<SystemT*>(std::as_const(*this).tryGet<SystemT>());
	}

	template<typename SystemT>
	[[nodiscard]]
	const SystemT* tryGet() const {
		const auto it = systems.find(get_type_index<SystemT>());

		if (it != systems.end()) {
			return static_cast<const SystemT*>(it->second.get());
		}
		return nullptr;
	}

	// Set a system's priority. Higher priority systems get executed sooner.
	template<typename SystemT>
	void setSystemPriority(u32 priority) {
		const auto it = systems.find(get_type_index<SystemT>());
		if (it != systems.end()) {
			it->second->setPriority(priority);
			sortSystemQueue();
		}
	}

private:

	void sortSystemQueue() {
		std::sort(system_queue.begin(), system_queue.end(), PriorityCompareGreater());
	}

	struct PriorityCompareGreater {
		bool operator()(const System& lhs, const System& rhs) const {
			return lhs.getPriority() > rhs.getPriority();
		}
	};

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The systems, mapped to their type_index
	std::unordered_map<std::type_index, std::unique_ptr<System>> systems;

	// The system execution order, determined by the system's priority.
	std::vector<std::reference_wrapper<System>> system_queue;
};


} // namespace ecs
