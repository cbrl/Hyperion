#pragma once

#include "entity_mgr.h"
#include "component_mgr.h"


class ECS final {
	public:
		ECS() {
			assert(initialized == false && "Calling ECS constructor after it has been initialized!");

			component_mgr = make_shared<ComponentMgr>();
			entity_mgr = make_unique<EntityMgr>(component_mgr);

			initialized = true;
		}

		~ECS() {
			initialized = false;
			entity_mgr.reset();
			component_mgr.reset();
		}


		template<typename EntityT, typename... ArgsT>
		static Handle64 CreateEntity(ArgsT&&... args) {
			return entity_mgr->CreateEntity<EntityT>(std::forward(args)...);
		}

		static void DestroyEntity(Handle64 entity) {
			entity_mgr->DestroyEntity(entity);
		}

		template<typename ComponentT, typename... ArgsT>
		static void AddComponent(Handle64 entity, ArgsT&&... args) {
			entity_mgr->GetEntity(entity)->AddComponent<ComponentT>(std::forward(args)...);
		}

		template<typename ComponentT>
		static void RemoveComponent(Handle64 entity) {
			entity_mgr->GetEntity(entity)->RemoveComponent<ComponentT>();
		}

		template<typename ComponentT>
		static ComponentT* GetComponent(Handle64 entity) {
			return entity_mgr->GetEntity(entity)->GetComponent<ComponentT>();
		}


	private:
		static bool initialized;

		static unique_ptr<EntityMgr>    entity_mgr;
		static shared_ptr<ComponentMgr> component_mgr;
};


// Initialize static members
bool                     ECS::initialized = false;
unique_ptr<EntityMgr>    ECS::entity_mgr;
shared_ptr<ComponentMgr> ECS::component_mgr;