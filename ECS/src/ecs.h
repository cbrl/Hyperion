#pragma once

#include "datatypes/scalar_types.h"
#include "time/time.h"
#include "memory/handle/handle.h"
#include <functional>


namespace ecs {

class EventMgr;
class SystemMgr;
class ComponentMgr;
class EntityMgr;

class Entity;
class IComponent;
class ISystem;

class ECS final {
	friend class EntityMgr;
	friend class ComponentMgr;
	friend class SystemMgr;

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
	~ECS() = default;


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
	handle64 createEntity();

	// Destroy an existing entity
	void removeEntity(handle64 entity);

	// Retrieve an entity using its handle
	[[nodiscard]]
	Entity& getEntity(handle64 handle);

	// Retrieve an entity using its handle
	[[nodiscard]]
	const Entity& getEntity(handle64 handle) const;

	// Attempt to retrieve an entity given its handle. Returns nullptr if given an invalid handle.
	[[nodiscard]]
	Entity* tryGetEntity(handle64 handle);

	// Attempt to retrieve an entity given its handle. Returns nullptr if given an invalid handle.
	[[nodiscard]]
	const Entity* tryGetEntity(handle64 handle) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	// Add a component to this entity
	template<typename ComponentT, typename... ArgsT>
	ComponentT& addComponent(handle64 entity, ArgsT&& ... args);

	// Get the first component of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT& getComponent(handle64 entity);

	// Get the first component of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT& getComponent(handle64 entity) const;

	// Get the first component of the specified type, if it exists.
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* tryGetComponent(handle64 entity);

	// Get the first component of the specified type, if it exists.
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT* tryGetComponent(handle64 entity) const;

	// Check if a given handle is valid
	[[nodiscard]]
	bool valid(handle64 handle) const;

	// Remove a component from this entity
	template<typename ComponentT>
	void removeComponent(handle64 entity);

	void removeComponent(handle64 entity, IComponent& component);

	// Check if this entity contains the specified component
	template<typename ComponentT>
	[[nodiscard]]
	bool hasComponent(handle64 entity) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	template<typename SystemT, typename... ArgsT>
	SystemT& addSystem(ArgsT&&... args);

	void removeSystem(ISystem& system);

	template<typename SystemT>
	void removeSystem();

	template<typename SystemT>
	[[nodiscard]]
	SystemT* tryGetSystem() const;

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
	void forEach(std::function<void(Entity&)> act);

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT>
	void forEach(std::function<void(const Entity&)> act) const;

	// Do something with each component of type ComponentT
	template<typename ComponentT>
	void forEach(std::function<void(ComponentT&)> act);

	// Do something with each component of type ComponentT
	template<typename ComponentT>
	void forEach(std::function<void(const ComponentT&)> act) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Count
	//----------------------------------------------------------------------------------

	// Get the number of a entities, or components of type T.
	template<typename T>
	[[nodiscard]]
	size_t count() const;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unique_ptr<EventMgr>     event_mgr;
	std::unique_ptr<SystemMgr>    system_mgr;
	std::unique_ptr<ComponentMgr> component_mgr;
	std::unique_ptr<EntityMgr>    entity_mgr;
};

} // namespace ecs

#include "ecs.tpp"