#include "ecs.h"


namespace ecs {

template <typename SystemT, typename... ArgsT>
SystemT& SystemMgr::add(ArgsT&&... args) {
	static_assert(std::is_base_of_v<ISystem, SystemT>, "SystemT must inherit from System.");

	static_assert(std::is_constructible_v<SystemT, ArgsT...>,
	              "SystemT does not have a constructor taking the provided argument types.");

	const auto it = systems.find(SystemT::index);

	// Return the system if it has already been added
	if (it != systems.end() && it->second != nullptr)
		return static_cast<SystemT&>(*(it->second));

	// Create the system
	auto  pair   = systems.try_emplace(SystemT::index, std::make_unique<SystemT>(ecs.get(), std::forward<ArgsT>(args)...));
	auto& system = static_cast<SystemT&>(*(pair.first->second));

	// Add the system to the queue and sort it
	system_queue.push_back(std::ref(system));
	sortSystemQueue();

	return system;
}


template<typename SystemT>
void SystemMgr::remove() {
	// Remove from the array of systems
	systems.erase(SystemT::index);

	// Remove from the sorted system queue
	const auto it = std::find_if(system_queue.begin(), system_queue.end(),
		[](const ISystem& system) {
			return system.getTypeIndex() == SystemT::index;
		}
	);

	if (it != system_queue.end()) {
		system_queue.erase(it);
	}
}


template<typename SystemT>
SystemT& SystemMgr::get() {
	return static_cast<SystemT&>(systems.at(SystemT::index));
}


template<typename SystemT>
const SystemT& SystemMgr::get() const {
	return static_cast<const SystemT&>(systems.at(SystemT::index));
}


template<typename SystemT>
SystemT* SystemMgr::tryGet() {
	const auto it = systems.find(SystemT::index);

	if (it != systems.end()) {
		return static_cast<SystemT*>(it->second.get());
	}
	return nullptr;
}


template<typename SystemT>
const SystemT* SystemMgr::tryGet() const {
	const auto it = systems.find(SystemT::index);

	if (it != systems.end()) {
		return static_cast<const SystemT*>(it->second.get());
	}
	return nullptr;
}


template<typename SystemT>
void SystemMgr::setSystemPriority(u32 priority) {
	const auto it = systems.find(SystemT::index);
	if (it != systems.end()) {
		it->second->setPriority(priority);
		sortSystemQueue();
	}
}

} // namespace ecs