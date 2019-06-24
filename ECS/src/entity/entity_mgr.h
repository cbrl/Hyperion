#pragma once

#include "entity/entity.h"
#include "memory/handle/handle_map.h"

namespace ecs {

class ComponentMgr;
class EventMgr;

//----------------------------------------------------------------------------------
// Entity Manager
//----------------------------------------------------------------------------------
//
// Handles the creation and destruction of entities
//
//----------------------------------------------------------------------------------

class EntityMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EntityMgr(std::shared_ptr<ComponentMgr> component_mgr, EventMgr& event_mgr);
	EntityMgr(const EntityMgr& manager) = delete;
	EntityMgr(EntityMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EntityMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EntityMgr& operator=(const EntityMgr& manager) = delete;
	EntityMgr& operator=(EntityMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Entities
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	EntityPtr createEntity();

	// Add an entity to the list of expired entities. Will be
	// destroyed at the end of the next ECS update.
	void destroyEntity(handle64 handle);

	// Remove all the entities marked for deletion. Should be called once per tick.
	void removeExpiredEntities();

	// Get the entity associated with the handle
	[[nodiscard]]
	Entity* getEntity(handle64 handle);

	// Get the number of entities
	[[nodiscard]]
	size_t count() const noexcept;

	// Check if a handle is valid
	[[nodiscard]]
	bool valid(handle64 entity) const noexcept;


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each entity
	void forEach(const std::function<void(Entity&)>& act);

	// Apply an action to each entity
	void forEach(const std::function<void(const Entity&)>& act) const;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A pointer to the component manager
	std::shared_ptr<ComponentMgr> component_mgr;

	// A reference to the event manager. Passed to the entity (EventSender).
	EventMgr& event_mgr;

	// Handle map. Maps a handle to an Entity.
	HandleMap<handle64, Entity> entity_map;

	// A container of entities that need to be deleted
	std::vector<handle64> expired_entities;
};

} // namespace ecs