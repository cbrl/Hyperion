#pragma once

#include "util\engine_util.h"
#include "ecs\entity\entity_mgr.h"
#include "ecs\component\component_mgr.h"
#include "ecs\system\system_mgr.h"


class ECS final {
	public:
		ECS();
		~ECS();


		static ECS* const Get() {
			return engine;
		}


		void Update(float delta_time) {
			system_mgr->Update(delta_time);
		}


		template<typename EntityT, typename... ArgsT>
		[[nodiscard]]
		Handle64 CreateEntity(ArgsT&&... args) {
			static_assert(std::is_base_of_v<IEntity, EntityT>,
						  "Calling ECS::CreateEntity() with non-entity type.");

			return entity_mgr->CreateEntity<EntityT>(std::forward<ArgsT>(args)...);
		}


		void DestroyEntity(Handle64 entity) {
			entity_mgr->DestroyEntity(entity);
		}


		template<typename ComponentT, typename... ArgsT>
		ComponentT* AddComponent(Handle64 entity, ArgsT&&... args) {
			static_assert(std::is_base_of_v<IComponent, ComponentT>,
						  "Calling ECS::AddComponent() with non-component type.");

			entity_mgr->GetEntity(entity)->AddComponent<ComponentT>(std::forward<ArgsT>(args)...);
		}


		template<typename ComponentT>
		void RemoveComponent(Handle64 entity) {
			static_assert(std::is_base_of_v<IComponent, ComponentT>,
						  "Calling ECS::RemoveComponent() with non-component type.");

			entity_mgr->GetEntity(entity)->RemoveComponent<ComponentT>();
		}


		template<typename ComponentT>
		[[nodiscard]]
		ComponentT* GetComponent(Handle64 entity) {
			static_assert(std::is_base_of_v<IComponent, ComponentT>,
						  "Calling ECS::GetComponent() with non-component type.");

			return entity_mgr->GetEntity(entity)->GetComponent<ComponentT>();
		}


		template<typename SystemT, typename... ArgsT>
		SystemT* AddSystem(ArgsT&&... args) {
			static_assert(std::is_base_of_v<ISystem, SystemT>,
						  "Calling ECS::AddSystem() with non-system type.");

			return system_mgr->AddSystem<SystemT>(std::forward<ArgsT>(args)...);
		}


		// Do something to each entity or component
		template<typename T, typename ActionT>
		void ForEach(ActionT act) {
			if constexpr (std::is_base_of_v<IEntity, T>) {
				if (entity_mgr->KnowsEntity<T>()) return;

				for (auto entity = entity_mgr->begin<T>(); entity != entity_mgr->end<T>(); ++entity) {
					if (entity->IsActive()) act(*entity);
				}
			}

			if constexpr (std::is_base_of_v<IComponent, T>) {
				if (!component_mgr->KnowsComponent<T>()) return;

				for (auto component = component_mgr->begin<T>(); component != component_mgr->end<T>(); ++component) {
					if (component->IsActive()) act(*component);
				}
			}
		}


		EntityMgr*    GetEntityMgr()    const { return entity_mgr.get(); }
		ComponentMgr* GetComponentMgr() const { return component_mgr.get(); }


	private:
		static ECS* engine;

		unique_ptr<EntityMgr>    entity_mgr;
		unique_ptr<ComponentMgr> component_mgr;
		unique_ptr<SystemMgr>    system_mgr;
};