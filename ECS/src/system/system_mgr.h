#pragma once

#include "memory/allocator/linear_allocator.h"
#include "system/system.h"
#include "exception/exception.h"


class SystemMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	SystemMgr();
	SystemMgr(const SystemMgr& manager) = delete;
	SystemMgr(SystemMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~SystemMgr();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	SystemMgr& operator=(const SystemMgr& manager) = delete;
	SystemMgr& operator=(SystemMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

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
	unique_ptr<LinearAllocator> allocator;
	static constexpr size_t alloc_memory_size = 4194304;

	std::unordered_map<std::type_index, ISystem*> systems;
};
