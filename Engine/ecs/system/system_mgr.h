#pragma once

#include "util\memory\allocator\linear_allocator.h"
#include "ecs\system\system.h"


class SystemMgr final {
	public:
		SystemMgr();
		~SystemMgr();

		void Update(const Engine& engine);

		template<typename SystemT, typename... ArgsT>
		SystemT* AddSystem(ArgsT&&... args) {
			const auto& it = systems.find(SystemT::type_id);

			if (it != systems.end() && it->second != nullptr)
				return static_cast<SystemT*>(it->second);

			void* memory = allocator->Allocate(sizeof(SystemT), alignof(SystemT));

			if (memory != nullptr) {
				SystemT* sys = new(memory) SystemT(std::forward<ArgsT>(args)...);

				systems[SystemT::type_id] = sys;
				return sys;
			}

			assert(true && "SystemMgr::AddSystem() - Unable to allocate memory for System.");
		}

		template<typename SystemT>
		SystemT* GetSystem() const {
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