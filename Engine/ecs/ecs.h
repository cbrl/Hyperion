#pragma once

#include "util\engine_util.h"
#include "ecs\entity\entity_mgr.h"
#include "ecs\component\component_mgr.h"
#include "ecs\system\system_mgr.h"


class ECS final {
	public:
		ECS();
		~ECS();

		void Update(const Engine& engine);


		//----------------------------------------------------------------------------------
		// Entities
		//----------------------------------------------------------------------------------

		template<typename EntityT, typename... ArgsT>
		[[nodiscard]]
		const Handle64 CreateEntity(ArgsT&&... args);

		void DestroyEntity(Handle64 entity);


		//----------------------------------------------------------------------------------
		// Components
		//----------------------------------------------------------------------------------

		template<typename ComponentT, typename... ArgsT>
		ComponentT* const AddComponent(Handle64 entity, ArgsT&&... args);

		template<typename ComponentT>
		void RemoveComponent(Handle64 entity);

		template<typename ComponentT>
		[[nodiscard]]
		ComponentT* const GetComponent(Handle64 entity);


		//----------------------------------------------------------------------------------
		// Systems
		//----------------------------------------------------------------------------------

		template<typename SystemT, typename... ArgsT>
		SystemT* const AddSystem(ArgsT&&... args);


		//----------------------------------------------------------------------------------
		// Misc
		//----------------------------------------------------------------------------------

		// Do something to each active entity or component
		template<typename T, typename ActionT>
		void ForEachActive(ActionT act);

		// Get the number of a specific entity/component
		template<typename T>
		size_t CountOf() const;

		EntityMgr* const    GetEntityMgr()    const { return entity_mgr.get(); }
		ComponentMgr* const GetComponentMgr() const { return component_mgr.get(); }


	private:
		unique_ptr<EntityMgr>    entity_mgr;
		shared_ptr<ComponentMgr> component_mgr;
		unique_ptr<SystemMgr>    system_mgr;
};


#include "ecs.tpp"