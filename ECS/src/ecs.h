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

	// Update the systems. Should be called once per iteration of the main program loop.
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

	// Add a component to the entity
	template<typename ComponentT, typename... ArgsT> requires std::derived_from<ComponentT, IComponent>
	ComponentT& add(handle64 entity, ArgsT&& ... args);

	// Remove a component from the entity
	template<typename ComponentT> requires std::derived_from<ComponentT, IComponent>
	void remove(handle64 entity);

	// Remove a component from the entity
	void remove(handle64 entity, IComponent& component);

	// Get the component of the specified type
	template<typename ComponentT> requires std::derived_from<ComponentT, IComponent>
	[[nodiscard]]
	ComponentT& get(handle64 entity);

	// Get the component of the specified type
	template<typename ComponentT> requires std::derived_from<ComponentT, IComponent>
	[[nodiscard]]
	const ComponentT& get(handle64 entity) const;

	// Get the component of the specified type, if it exists.
	template<typename ComponentT> requires std::derived_from<ComponentT, IComponent>
	[[nodiscard]]
	ComponentT* tryGet(handle64 entity);

	// Get the component of the specified type, if it exists.
	template<typename ComponentT> requires std::derived_from<ComponentT, IComponent>
	[[nodiscard]]
	const ComponentT* tryGet(handle64 entity) const;

	// Check if the entity contains the specified component
	template<typename ComponentT> requires std::derived_from<ComponentT, IComponent>
	[[nodiscard]]
	bool has(handle64 entity) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	// Add a system to the ECS
	template<typename SystemT, typename... ArgsT> requires std::derived_from<SystemT, ISystem>
	SystemT& add(ArgsT&&... args);

	// Remove a system from the ECS
	void remove(ISystem& system);

	// Remove a system from the ECS
	template<typename SystemT> requires std::derived_from<SystemT, ISystem>
	void remove();

	// Get the specified system
	template<typename SystemT> requires std::derived_from<SystemT, ISystem>
	[[nodiscard]]
	SystemT& get();

	// Get the specified system
	template<typename SystemT> requires std::derived_from<SystemT, ISystem>
	[[nodiscard]]
	const SystemT& get() const;

	// Get the specified system, if it exists.
	template<typename SystemT> requires std::derived_from<SystemT, ISystem>
	[[nodiscard]]
	SystemT* tryGet();

	// Get the specified system, if it exists.
	template<typename SystemT> requires std::derived_from<SystemT, ISystem>
	[[nodiscard]]
	const SystemT* tryGet() const;

	// Set the priority of the specified system
	template<typename SystemT> requires std::derived_from<SystemT, ISystem>
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

	// Get the dispatcher for the specified event type
	template<typename EventT>
	[[nodiscard]]
	EventDispatcher<EventT>& getDispatcher();

	// Remove a registered callback
	template<typename EventT>
	void removeCallback(const std::function<void(const EventT&)>& callback);


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT> requires (std::derived_from<ComponentT, IComponent> && ...)
	void forEach(std::function<void(handle64)> act);

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT> requires (std::derived_from<ComponentT, IComponent> && ...)
	void forEach(std::function<void(handle64)> act) const;

	// Do something with each component of type ComponentT
	template<typename ComponentT> requires std::derived_from<ComponentT, IComponent>
	void forEach(std::function<void(ComponentT&)> act);

	// Do something with each component of type ComponentT
	template<typename ComponentT> requires std::derived_from<ComponentT, IComponent>
	void forEach(std::function<void(const ComponentT&)> act) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Count
	//----------------------------------------------------------------------------------

	// Get the number of a entities or components of type T.
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