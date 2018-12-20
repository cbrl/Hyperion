template<typename KeyT, typename ValueT>
template<typename... ArgsT>
std::shared_ptr<ValueT> ManagedResourceMap<KeyT, ValueT>::getOrCreateResource(const KeyT& key, ArgsT&&... args) noexcept {

	// Find the resource
	const auto it = resource_map.find(key);

	// Check if the resource exists
	if (it != resource_map.end()) {

		// Return the resource if it hasn't expired
		if (!it->second.expired()) {
			return it->second.lock();
		}

		// If the resource has expired then remove the ptr from the map
		resource_map.erase(it);
	}

	// Create the resource if it doesn't exist or expired
	const auto resource = std::make_shared<ValueT>(std::forward<ArgsT>(args)...);
	resource_map[key] = resource;

	return resource;
}


template<typename KeyT, typename ValueT>
std::shared_ptr<ValueT> ManagedResourceMap<KeyT, ValueT>::getResource(const KeyT& key) const noexcept {

	const auto n = resource_map.find(key);

	if (n != resource_map.end()) {

		if (!n->second.expired()) {
			return n->second.lock();
		}

		// Erase the pointer if it expired
		resource_map.erase(n);
	}

	// Return a null pointer if the resource doesn't exist
	return std::shared_ptr<ValueT>();
}
