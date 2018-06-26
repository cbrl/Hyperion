#pragma once

#include "memory/allocator/linear_allocator.h"
#include "system/system.h"


class SystemMgr final {
public:
	SystemMgr();
	~SystemMgr();

	void update(const Engine& engine);

	template<typename SystemT, typename... ArgsT>
	SystemT* addSystem(ArgsT&&... args) {
		const auto& it = systems.find(SystemT::type_id);

		if (it != systems.end() && it->second != nullptr)
			return static_cast<SystemT*>(it->second);

		void* memory = allocator->allocate<SystemT>();

		if (memory != nullptr) {
			SystemT* sys = new(memory) SystemT(std::forward<ArgsT>(args)...);

			systems[SystemT::type_id] = sys;
			return sys;
		}

		assert("SystemMgr::AddSystem() - Unable to allocate memory for System.");

		return nullptr;
	}

	template<typename SystemT>
	SystemT* getSystem() const {
		const auto& it = systems.find(SystemT::type_id);

		if (it != systems.end() && it->second != nullptr)
			return static_cast<SystemT*>(it->second);

		return nullptr;
	}


private:
	LinearAllocator* allocator;
	static constexpr size_t alloc_memory_size = 4194304;

	unordered_map<type_index, ISystem*> systems;
};
