#pragma once

#include "datatypes/scalar_types.h"
#include "memory/handle/handle.h"
#include "time/time.h"
#include "event/events.h"
#include "event/event_dispatcher.h"

#include <functional>


namespace ecs {

class EventMgr;
class SystemMgr;
class ComponentMgr;
class EntityMgr;

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
	// Member Functions - Update
	//----------------------------------------------------------------------------------

	// Update the systems
	void update(std::chrono::duration<f64> dt);


	//----------------------------------------------------------------------------------
	// Member Functions - Entities
	//----------------------------------------------------------------------------------

	// Create a new entity
	[[nodiscard]]
	handle64 create();

	// Destroy an existing entity
	void destroy(handle64 entity);

	// Check if a given handle is valid
	[[nodiscard]]
	bool valid(handle64 handle) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	// Add a component to this entity
	template<typename ComponentT, typename... ArgsT, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentT>>>
	ComponentT& add(handle64 entity, ArgsT&& ... args);

	// Remove a component from this entity
	template<typename ComponentT, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentT>>>
	void remove(handle64 entity);

	void remove(handle64 entity, IComponent& component);

	// Get the first component of the specified type
	template<typename ComponentT, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentT>>>
	[[nodiscard]]
	ComponentT& get(handle64 entity);

	// Get the first component of the specified type
	template<typename ComponentT, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentT>>>
	[[nodiscard]]
	const ComponentT& get(handle64 entity) const;

	// Get the first component of the specified type, if it exists.
	template<typename ComponentT, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentT>>>
	[[nodiscard]]
	ComponentT* tryGet(handle64 entity);

	// Get the first component of the specified type, if it exists.
	template<typename ComponentT, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentT>>>
	[[nodiscard]]
	const ComponentT* tryGet(handle64 entity) const;

	// Check if this entity contains the specified component
	template<typename ComponentT>
	[[nodiscard]]
	bool has(handle64 entity) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	template<typename SystemT, typename... ArgsT, typename = std::enable_if_t<std::is_base_of_v<ISystem, SystemT>>>
	SystemT& add(ArgsT&&... args);

	void remove(ISystem& system);

	template<typename SystemT, typename = std::enable_if_t<std::is_base_of_v<ISystem, SystemT>>>
	void remove();

	template<typename SystemT, typename = std::enable_if_t<std::is_base_of_v<ISystem, SystemT>>>
	[[nodiscard]]
	SystemT& get();

	template<typename SystemT, typename = std::enable_if_t<std::is_base_of_v<ISystem, SystemT>>>
	[[nodiscard]]
	const SystemT& get() const;

	template<typename SystemT, typename = std::enable_if_t<std::is_base_of_v<ISystem, SystemT>>>
	[[nodiscard]]
	SystemT* tryGet();

	template<typename SystemT, typename = std::enable_if_t<std::is_base_of_v<ISystem, SystemT>>>
	[[nodiscard]]
	const SystemT* tryGet() const;

	template<typename SystemT>
	void setSystemPriority(u32 priority);


	//----------------------------------------------------------------------------------
	// Member Functions - Events
	//----------------------------------------------------------------------------------

	// Enqueue an event to be received by all listeners
	template<typename EventT, typename... ArgsT>
	void enqueue(ArgsT&&... args);

	// Immediately send an event to all listeners
	template<typename EventT, typename... ArgsT>
	void send(ArgsT&&... args);

	// Register a free function callback that listens for events of type EventT
	template<typename EventT, auto Function>
	DispatcherConnection addCallback();

	// Register a class member function callback that listens for events of type EventT
	template<typename EventT, auto Function, typename ClassT>
	DispatcherConnection addCallback(ClassT* instance);

	// Remove a registered callback
	template<typename EventT>
	void removeCallback(const std::function<void(const EventT&)>& callback);


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT>
	void forEach(std::function<void(handle64)> act);

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT>
	void forEach(std::function<void(handle64)> act) const;

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