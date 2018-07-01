#pragma once

#include "entity/entity_mgr.h"
#include "component/component_mgr.h"
#include "system/system_mgr.h"


class ECS final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ECS();
	ECS(const ECS& ecs) = delete;
	ECS(ECS&& ecs) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ECS();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ECS& operator=(const ECS& ecs) = delete;
	ECS& operator=(ECS&& ecs) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Entities
	//----------------------------------------------------------------------------------

	template<typename EntityT, typename... ArgsT>
	[[nodiscard]]
	handle64 createEntity(ArgsT&&... args);

	void destroyEntity(handle64 entity) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	template<typename ComponentT, typename... ArgsT>
	ComponentT* addComponent(handle64 entity, ArgsT&&... args);

	template<typename ComponentT>
	void removeComponent(handle64 entity) const;

	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* getComponent(handle64 entity) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	template<typename SystemT, typename... ArgsT>
	SystemT* addSystem(ArgsT&&... args);

	// Update the systems
	void update(const Engine& engine) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Misc
	//----------------------------------------------------------------------------------

	// Do something with each entity or component of type T
	template<typename T, typename ActionT>
	void forEach(ActionT act);

	// Get the number of a specific entity/component
	template<typename T>
	size_t countOf() const;

	[[nodiscard]]
	EntityMgr* getEntityMgr() const { return entity_mgr.get(); }

	[[nodiscard]]
	ComponentMgr* getComponentMgr() const { return component_mgr.get(); }


private:
	unique_ptr<EntityMgr> entity_mgr;
	shared_ptr<ComponentMgr> component_mgr;
	unique_ptr<SystemMgr> system_mgr;
};


#include "ecs.tpp"
