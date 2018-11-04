
template<typename EntityT, typename... ArgsT>
EntityPtr EntityMgr::createEntity(ArgsT&&... args) {

	static_assert(std::is_base_of_v<Entity, EntityT>,
	              "Calling EntityMgr::CreateEntity() with non-entity type.");

	void* memory = entity_pool.allocate();

	// Return an invalid pointer if memory allocation failed
	if (!memory) {
		return EntityPtr{this, handle64::invalid_handle};
	}

	// Create a handle
	const handle64  handle = handle_table.createHandle(static_cast<Entity*>(memory));
	const EntityPtr ptr = EntityPtr{this, handle};

	// Create the entity
	new(memory) EntityT(ptr, gsl::make_not_null(component_mgr.get()), std::forward<ArgsT>(args)...);

	return ptr;
}