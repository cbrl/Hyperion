template <typename SystemT, typename... ArgsT>
SystemT& SystemMgr::addSystem(ArgsT&&... args) {
	const auto it = systems.find(SystemT::index);

	if (it != systems.end() && it->second != nullptr)
		return static_cast<SystemT&>(*(it->second));

	auto  pair   = systems.try_emplace(SystemT::index, std::make_unique<SystemT>(std::forward<ArgsT>(args)...));
	auto& system = static_cast<SystemT&>(*(pair.first->second));

	if constexpr (std::is_base_of_v<EventParticipator, SystemT>) {
		system.setEventMgr(gsl::make_not_null(&event_mgr));
		if constexpr (std::is_base_of_v<EventListener, SystemT>) {
			system.doRegisterCallbacks();
		}
	}
	
	system.setECS(gsl::make_not_null(&ecs));

	system_queue.push_back(std::ref(system));
	sortSystemQueue();

	return system;
}


template<typename SystemT>
void SystemMgr::removeSystem() {
	systems.erase(SystemT::index);

	const auto it = std::find_if(system_queue.begin(), system_queue.end(),
		[](const ISystem& system) {
			return system.getTypeIndex() == SystemT::index;
		}
	);

	if (it != system_queue.end()) {
		system_queue.erase();
	}
}


template <typename SystemT>
SystemT* SystemMgr::getSystem() const {
	const auto it = systems.find(SystemT::index);

	if (it != systems.end()) {
		return static_cast<SystemT*>(it->second.get());
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