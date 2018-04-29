#pragma once

#include "util\engine_util.h"
#include "ecs\entity\entity_mgr.h"
#include "ecs\component\component_mgr.h"


class ECS final {
	public:
		ECS();
		~ECS();

		template<typename EntityT, typename... ArgsT>
		Handle64 CreateEntity(ArgsT&&... args) {
			return entity_mgr->CreateEntity<EntityT>(std::forward(args)...);
		}

		void DestroyEntity(Handle64 entity) {
			entity_mgr->DestroyEntity(entity);
		}

		template<typename ComponentT, typename... ArgsT>
		void AddComponent(Handle64 entity, ArgsT&&... args) {
			entity_mgr->GetEntity(entity)->AddComponent<ComponentT>(std::forward(args)...);
		}

		template<typename ComponentT>
		void RemoveComponent(Handle64 entity) {
			entity_mgr->GetEntity(entity)->RemoveComponent<ComponentT>();
		}

		template<typename ComponentT>
		ComponentT* GetComponent(Handle64 entity) {
			return entity_mgr->GetEntity(entity)->GetComponent<ComponentT>();
		}


	public:
		static ECS* Engine;


	private:
		unique_ptr<EntityMgr>    entity_mgr;
		shared_ptr<ComponentMgr> component_mgr;
};