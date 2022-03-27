module;

#include <concepts>
#include <memory>
#include <functional>
#include <type_traits>

#include "memory/handle/handle.h"
#include "time/time.h"

export module ecs:ecs_core;

import :component;
import :entity_mgr;
import :event_dispatcher;
import :event_mgr;
import :events;
import :system;


export namespace ecs {

class ECS final {
	friend class EntityMgr;
	friend class ComponentMgr;
	friend class SystemMgr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ECS() {
		event_mgr     = std::make_unique<EventMgr>();
		system_mgr    = std::make_unique<SystemMgr>();
		component_mgr = std::make_unique<ComponentMgr>(*event_mgr);
		entity_mgr    = std::make_unique<EntityMgr>(*component_mgr, *event_mgr);
	}

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
	void update(std::chrono::duration<f64> dt) {
		system_mgr->update(dt);
		event_mgr->dispatch();
		entity_mgr->removeExpiredEntities();
		//component_mgr->removeExpiredComponents();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Entities
	//----------------------------------------------------------------------------------

	// Create a new entity
	[[nodiscard]]
	handle64 create() {
		return entity_mgr->create();
	}

	// Destroy an existing entity
	void destroy(handle64 entity) {
		entity_mgr->destroyEntity(entity);
	}

	// Check if a given handle is valid
	[[nodiscard]]
	bool valid(handle64 handle) const {
		return entity_mgr->valid(handle);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	// Add a component to the entity
	template<typename ComponentT, typename... ArgsT> requires std::derived_from<ComponentT, Component>
	ComponentT& add(handle64 entity, ArgsT&&... args) {
		return component_mgr->add<ComponentT>(entity, std::forward<ArgsT>(args)...);
	}

	// Remove a component from the entity
	template<typename ComponentT> requires std::derived_from<ComponentT, Component>
	void remove(handle64 entity) {
		component_mgr->remove<ComponentT>(entity);
	}

	// Remove a component from the entity
	void remove(handle64 entity, Component& component) {
		component_mgr->remove(entity, component);
	}

	// Get the component of the specified type
	template<typename ComponentT> requires std::derived_from<ComponentT, Component>
	[[nodiscard]]
	ComponentT& get(handle64 entity) {
		return component_mgr->get<ComponentT>(entity);
	}

	// Get the component of the specified type
	template<typename ComponentT> requires std::derived_from<ComponentT, Component>
	[[nodiscard]]
	const ComponentT& get(handle64 entity) const {
		return component_mgr->get<ComponentT>(entity);
	}

	// Get the component of the specified type, if it exists.
	template<typename ComponentT> requires std::derived_from<ComponentT, Component>
	[[nodiscard]]
	ComponentT* tryGet(handle64 entity) {
		return component_mgr->tryGet<ComponentT>(entity);
	}

	// Get the component of the specified type, if it exists.
	template<typename ComponentT> requires std::derived_from<ComponentT, Component>
	[[nodiscard]]
	const ComponentT* tryGet(handle64 entity) const {
		return component_mgr->tryGet<ComponentT>(entity);
	}

	// Check if the entity contains the specified component
	template<typename ComponentT> requires std::derived_from<ComponentT, Component>
	[[nodiscard]]
	bool has(handle64 entity) const {
		return component_mgr->has<ComponentT>(entity);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Systems
	//----------------------------------------------------------------------------------

	// Add a system to the ECS
	template<typename SystemT, typename... ArgsT> requires std::derived_from<SystemT, System>
	SystemT& add(ArgsT&&... args) {
		return system_mgr->add<SystemT>(*this, std::forward<ArgsT>(args)...);
	}

	// Remove a system from the ECS
	void remove(System& system) {
		system_mgr->remove(system);
	}

	// Remove a system from the ECS
	template<typename SystemT> requires std::derived_from<SystemT, System>
	void remove() {
		system_mgr->remove<SystemT>();
	}

	// Get the specified system
	template<typename SystemT> requires std::derived_from<SystemT, System>
	[[nodiscard]]
	SystemT& get() {
		return system_mgr->get<SystemT>();
	}

	// Get the specified system
	template<typename SystemT> requires std::derived_from<SystemT, System>
	[[nodiscard]]
	const SystemT& get() const {
		return system_mgr->get<SystemT>();
	}

	// Get the specified system, if it exists.
	template<typename SystemT> requires std::derived_from<SystemT, System>
	[[nodiscard]]
	SystemT* tryGet() {
		return system_mgr->tryGet<SystemT>();
	}

	// Get the specified system, if it exists.
	template<typename SystemT> requires std::derived_from<SystemT, System>
	[[nodiscard]]
	const SystemT* tryGet() const {
		return system_mgr->tryGet<SystemT>();
	}

	// Set the priority of the specified system
	template<typename SystemT> requires std::derived_from<SystemT, System>
	void setSystemPriority(u32 priority) {
		system_mgr->setSystemPriority<SystemT>(priority);
	}

	//----------------------------------------------------------------------------------
	// Member Functions - Events
	//----------------------------------------------------------------------------------

	// Enqueue an event to be received by all listeners
	template<typename EventT, typename... ArgsT>
	void enqueue(ArgsT&&... args) {
		event_mgr->enqueue<EventT>(std::forward<ArgsT>(args)...);
	}

	// Immediately send an event to all listeners
	template<typename EventT, typename... ArgsT>
	void send(ArgsT&&... args) {
		event_mgr->send<EventT>(std::forward<ArgsT>(args)...);
	}

	// Get the dispatcher for the specified event type
	template<typename EventT>
	[[nodiscard]]
	EventDispatcher<EventT>& getDispatcher() {
		return event_mgr->getDispatcher<EventT>();
	}

	// Remove a registered callback
	template<typename EventT>
	void removeCallback(const std::function<void(const EventT&)>& callback) {
		event_mgr->removeCallback(callback);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Do something with each entity with a component of type ComponentT.
	// Providing no template arguments will apply the action to every entity.
	template<typename... ComponentT> requires (std::derived_from<ComponentT, Component> && ...)
	void forEach(const std::function<void(handle64)>& act) const {
		if constexpr (sizeof...(ComponentT) == 0) {
			entity_mgr->forEach(act);
		}
		else {
			// Return early if a type was provided that has never been used
			if ((!component_mgr->knowsComponent<ComponentT>() || ...))
				return;

			// Iterate over all entities that contain the provided component types
			entity_mgr->forEach([ecs = std::cref(*this), &act](handle64 entity) {
				if ((ecs.get().has<ComponentT>(entity) && ...))
					act(entity);
			});
		}
	}

	// Do something with each component of type ComponentT
	template<typename ComponentT> requires std::derived_from<ComponentT, Component>
	void forEach(std::function<void(ComponentT&)> act) {
		if (!component_mgr->knowsComponent<ComponentT>())
			return;

		component_mgr->forEach<ComponentT>(act);
	}


	// Do something with each component of type ComponentT
	template<typename ComponentT> requires std::derived_from<ComponentT, Component>
	void forEach(std::function<void(const ComponentT&)> act) const {
		if (!component_mgr->knowsComponent<ComponentT>())
			return;

		component_mgr->forEach<ComponentT>(act);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Count
	//----------------------------------------------------------------------------------

	// Get the number of a entities or components of type T.
	template<typename T>
	[[nodiscard]]
	size_t count() const {
		if constexpr (std::is_same_v<handle64, T>)
			return entity_mgr->count();
		else
			return component_mgr->count<T>();
	}

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
