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

	// Create a new entity
	[[nodiscard]]
	EntityPtr createEntity();

	// Destroy an existing entity
	void destroyEntity(handle64 entity);


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	template<typename SystemT, typename... ArgsT>
	SystemT* addSystem(ArgsT&&... args);

	// Update the systems
	void update(Engine& engine);


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Do something with each entity or component of type T
	template<typename T, typename ActionT>
	void forEach(ActionT&& act);


	//----------------------------------------------------------------------------------
	// Member Functions - Count
	//----------------------------------------------------------------------------------

	// Get the number of a specific entity/component
	template<typename T>
	[[nodiscard]]
	size_t countOf() const;


private:
	unique_ptr<EntityMgr> entity_mgr;
	shared_ptr<ComponentMgr> component_mgr;
	unique_ptr<SystemMgr> system_mgr;
};


#include "ecs.tpp"
