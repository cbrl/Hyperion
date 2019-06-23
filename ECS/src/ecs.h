#pragma once

#include "entity/entity_mgr.h"
#include "component/component_mgr.h"
#include "system/system_mgr.h"
#include "event/event_mgr.h"

namespace ecs {

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
	[[nodiscard]] EntityPtr createEntity();

	// Destroy an existing entity
	void destroyEntity(handle64 entity);


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	//// Add a component to this entity
	//template<typename ComponentT, typename... ArgsT>
	//ComponentT& addComponent(handle64 entity, ArgsT&& ... args);

	//// Get the first component of the specified type, if it exists.
	//template<typename ComponentT>
	//[[nodiscard]]
	//ComponentT* getComponent(handle64 entity);

	//// Get the first component of the specified type, if it exists.
	//template<typename ComponentT>
	//[[nodiscard]]
	//const ComponentT* getComponent(handle64 entity) const;

	//// Remove a specific component from this entity
	//template<typename ComponentT>
	//void removeComponent(handle64 entity);

	//void removeComponent(handle64 entity, IComponent& component);

	//// Check if this entity contains the specified component
	//template<typename ComponentT>
	//[[nodiscard]]
	//bool hasComponent(handle64 entity) const;

	//// Get the number of components of the specified type
	//template<typename ComponentT>
	//[[nodiscard]]
	//size_t count(handle64 entity) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	template<typename SystemT, typename... ArgsT>
	[[nodiscard]] SystemT& addSystem(ArgsT&&... args);

	void removeSystem(ISystem& system);

	template<typename SystemT>
	void removeSystem();

	template<typename SystemT>
	SystemT* getSystem() const;

	template<typename SystemT>
	void setSystemPriority(u32 priority);

	// Send an event to be received by any systems that listen for it
	template<typename EventT, typename... ArgsT>
	void sendEvent(ArgsT&&... args);

	// Update the systems
	void update(std::chrono::duration<f64> dt);


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT>
	void forEach(const std::function<void(Entity&)>& act);

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT>
	void forEach(const std::function<void(const Entity&)>& act) const;

	// Do something with each component of type ComponentT
	template<typename ComponentT>
	void forEach(const std::function<void(ComponentT&)>& act);

	// Do something with each component of type ComponentT
	template<typename ComponentT>
	void forEach(const std::function<void(const ComponentT&)>& act) const;

	//----------------------------------------------------------------------------------
	// Member Functions - Count
	//----------------------------------------------------------------------------------

	// Get the number of a entities, or components of type T.
	template<typename T>
	[[nodiscard]] size_t countOf() const;


private:
	std::unique_ptr<EventMgr> event_mgr;
	std::unique_ptr<SystemMgr> system_mgr;
	std::shared_ptr<ComponentMgr> component_mgr;
	std::unique_ptr<EntityMgr> entity_mgr;
};

} // namespace ecs

#include "ecs.tpp"