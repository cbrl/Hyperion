#pragma once

#include "util\engine_util.h"
#include "ecs\entity\entity_mgr.h"
#include "ecs\component\component_mgr.h"


class ECS final {
	public:
		ECS();
		~ECS();

		template<typename EntityT, typename... ArgsT>
		static Handle64 CreateEntity(ArgsT&&... args) {
			ThrowIfFailed(initialized, "ECS not initialized yet!");
			return entity_mgr->CreateEntity<EntityT>(std::forward(args)...);
		}

		static void DestroyEntity(Handle64 entity) {
			ThrowIfFailed(initialized, "ECS not initialized yet!");
			entity_mgr->DestroyEntity(entity);
		}

		template<typename ComponentT, typename... ArgsT>
		static void AddComponent(Handle64 entity, ArgsT&&... args) {
			ThrowIfFailed(initialized, "ECS not initialized yet!");
			entity_mgr->GetEntity(entity)->AddComponent<ComponentT>(std::forward(args)...);
		}

		template<typename ComponentT>
		static void RemoveComponent(Handle64 entity) {
			ThrowIfFailed(initialized, "ECS not initialized yet!");
			entity_mgr->GetEntity(entity)->RemoveComponent<ComponentT>();
		}

		template<typename ComponentT>
		static ComponentT* GetComponent(Handle64 entity) {
			ThrowIfFailed(initialized, "ECS not initialized yet!");
			return entity_mgr->GetEntity(entity)->GetComponent<ComponentT>();
		}


	private:
		static bool initialized;

		static unique_ptr<EntityMgr>    entity_mgr;
		static shared_ptr<ComponentMgr> component_mgr;
};