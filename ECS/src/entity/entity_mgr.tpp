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


template<typename EntityT, typename ActionT>
void EntityMgr::forEach(ActionT act) {
	using pool_t = ResourcePool<EntityT>;

	auto* pool = static_cast<pool_t*>(entity_pools.getPool<EntityT>());
	pool->forEach(act);
}