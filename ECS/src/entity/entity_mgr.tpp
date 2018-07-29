template<typename EntityT, typename... ArgsT>
handle64 EntityMgr::createEntity(ArgsT&&... args) {

	static_assert(std::is_base_of_v<IEntity, EntityT>,
				  "Calling EntityMgr::CreateEntity() with non-entity type.");

	auto  pool = entity_pools.getOrCreatePool<EntityT>();
	void* memory = pool->allocateObject();

	// Create a handle
	handle64 handle = handle_table.createHandle(static_cast<IEntity*>(memory));

	// Create the entity
	new(memory) EntityT(handle, component_mgr.get(), std::forward<ArgsT>(args)...);

	return handle;
}


// Add an entity to the list of expired entities. Will be
// destroyed at the end of the next ECS update.
void EntityMgr::destroyEntity(handle64 handle) {

	if (expired_entities.size() > num_expired_entities) {
		expired_entities[num_expired_entities] = handle;
		++num_expired_entities;
	}
	else {
		expired_entities.push_back(handle);
		++num_expired_entities;
	}
}


// Remove all the entities marked for deletion. Should be
// called once per tick.
void EntityMgr::removeExpiredEntities() {
	for (const handle64 handle : expired_entities) {

		IEntity* entity = getEntity(handle);
		const auto type = entity->getTypeId();

		auto pool = entity_pools.getPool(type);

		// Destroy the entity
		handle_table.releaseHandle(handle);
		pool->destroyObject(entity);
	}

	expired_entities.clear();
	num_expired_entities = 0;
}
