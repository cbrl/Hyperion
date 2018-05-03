#pragma once

#include "util\engine_util.h"
#include "ecs\entity\entity_mgr.h"
#include "ecs\component\component_mgr.h"


class ECS final {
	public:
		ECS();
		~ECS();

		static ECS* const Get() {
			return engine;
		}

		template<typename EntityT, typename... ArgsT>
		Handle64 CreateEntity(ArgsT&&... args) {
			static_assert(std::is_base_of_v<IEntity, EntityT>,
						  "Calling ECS::CreateEntity() with non-component type.");

			return entity_mgr->CreateEntity<EntityT>(std::forward<ArgsT>(args)...);
		}

		void DestroyEntity(Handle64 entity) {
			entity_mgr->DestroyEntity(entity);
		}

		template<typename ComponentT, typename... ArgsT>
		void AddComponent(Handle64 entity, ArgsT&&... args) {
			static_assert(std::is_base_of_v<IComponent, ComponentT>,
						  "Calling ECS::AddComponent() with non-component type.");

			entity_mgr->GetEntity(entity)->AddComponent<ComponentT>(std::forward<ArgsT>(args)...);
		}

		template<typename ComponentT>
		void RemoveComponent(Handle64 entity) {
			entity_mgr->GetEntity(entity)->RemoveComponent<ComponentT>();
		}

		template<typename ComponentT>
		ComponentT* GetComponent(Handle64 entity) {
			return entity_mgr->GetEntity(entity)->GetComponent<ComponentT>();
		}

		// Do something to each entity or component
		template<typename T, typename ActionT>
		void ForEach(ActionT act) {
			if constexpr (std::is_base_of_v<IEntity, T>) {
				for (auto resource = entity_mgr->begin<T>(); resource != entity_mgr->end<T>(); ++resource) {
					act(resource);
				}
			}
			if constexpr (std::is_base_of_v<IComponent, T>) {
				for (auto resource = component_mgr->begin<T>(); resource != component_mgr->end<T>(); ++resource) {
					act(resource);
				}
			}
		}


	private:
		static ECS* engine;

		unique_ptr<EntityMgr>    entity_mgr;
		shared_ptr<ComponentMgr> component_mgr;
};