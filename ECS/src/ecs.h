#pragma once

#include "entity/entity_mgr.h"
#include "component/component_mgr.h"
#include "system/system_mgr.h"


class ECS final {
public:
	ECS();
	~ECS();

	void Update(const Engine& engine) const;


	//----------------------------------------------------------------------------------
	// Entities
	//----------------------------------------------------------------------------------

	template<typename EntityT, typename... ArgsT>
	[[nodiscard]]
	Handle64 CreateEntity(ArgsT&&... args);

	void DestroyEntity(Handle64 entity) const;


	//----------------------------------------------------------------------------------
	// Components
	//----------------------------------------------------------------------------------

	template<typename ComponentT, typename... ArgsT>
	ComponentT* AddComponent(Handle64 entity, ArgsT&&... args);

	template<typename ComponentT>
	void RemoveComponent(Handle64 entity) const;

	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* GetComponent(Handle64 entity) const;


	//----------------------------------------------------------------------------------
	// Systems
	//----------------------------------------------------------------------------------

	template<typename SystemT, typename... ArgsT>
	SystemT* AddSystem(ArgsT&&... args);


	//----------------------------------------------------------------------------------
	// Misc
	//----------------------------------------------------------------------------------

	// Do something to each active entity or component
	template<typename T, typename ActionT>
	void ForEachActive(ActionT act);

	// Get the number of a specific entity/component
	template<typename T>
	size_t CountOf() const;

	EntityMgr* GetEntityMgr() const { return entity_mgr.get(); }
	ComponentMgr* GetComponentMgr() const { return component_mgr.get(); }


private:
	unique_ptr<EntityMgr> entity_mgr;
	shared_ptr<ComponentMgr> component_mgr;
	unique_ptr<SystemMgr> system_mgr;
};


#include "ecs.tpp"
