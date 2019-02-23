#pragma once

#include "entity/entity_mgr.h"
#include "component/component_mgr.h"
#include "system/system_mgr.h"
#include "event/event_mgr.h"


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
	[[nodiscard]]
	SystemT& addSystem(ArgsT&&... args);

	void removeSystem(ISystem& system);

	template<typename SystemT>
	void removeSystem();

	// Update the systems
	void update(f64 dt);

	template<typename EventT, typename... ArgsT>
	void sendEvent(ArgsT&&... args);


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
	std::unique_ptr<EventMgr>     event_mgr;
	std::unique_ptr<SystemMgr>    system_mgr;
	std::shared_ptr<ComponentMgr> component_mgr;
	std::unique_ptr<EntityMgr>    entity_mgr;
};

#include "ecs.tpp"
