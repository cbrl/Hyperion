#pragma once

#include "util\datatypes\datatypes.h"
#include "util\memory\resource_pool.h"
#include "ecs\component.h"


class IComponentPool {};

template<typename ComponentT>
class ComponentPool final : public IComponentPool, public ResourcePool<ComponentT, 512> {};


class ComponentPoolMap {
	public:
		ComponentPoolMap() = default;
		~ComponentPoolMap() = default;

		template<typename ComponentT>
		ComponentPool<ComponentT>* GetOrCreatePool() {

			using pool_t = ComponentPool<ComponentT>;

			const auto it = pools.find(ComponentT::type_idx);

			if (it == pools.end()) {
				pools[ComponentT::type_idx] = make_unique<pool_t>();

				return static_cast<pool_t*>(pools[ComponentT::type_idx].get());
			}

			return static_cast<pool_t*>(it->second.get());
		}


	private:
		unordered_map<type_index, unique_ptr<IComponentPool>> pools;
};



class ComponentMgr {
	public:
		ComponentMgr() = default;
		~ComponentMgr() = default;


		template<typename ComponentT, typename... ArgsT>
		[[nodiscard]]
		ComponentT* CreateComponent(ArgsT&&... args) {

			auto table = component_pools.GetOrCreatePool<ComponentT>();

			void* memory = table->CreateObject();

			ComponentT* component = new (memory)ComponentT(std::forward(args)...);

			return component;
		}


		template<typename ComponentT>
		void DestroyComponent(ComponentT* component) {

			auto table = component_pools.GetOrCreatePool<ComponentT>();

			table->DestroyObject(reinterpret_cast<void*>(component));
		}


	private:
		ComponentPoolMap component_pools;
};
