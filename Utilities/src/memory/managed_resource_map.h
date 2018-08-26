#pragma once

#include "datatypes/pointer_types.h"
#include "datatypes/container_types.h"


template<typename KeyT, typename ValueT>
class ManagedResourceMap final {
public:
	using key_type = KeyT;
	using value_type = ValueT;


public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ManagedResourceMap() = default;
	ManagedResourceMap(const ManagedResourceMap<KeyT, ValueT>& map) = delete;
	ManagedResourceMap(ManagedResourceMap<KeyT, ValueT>&& map) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ManagedResourceMap() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ManagedResourceMap<KeyT, ValueT>& operator=(const ManagedResourceMap<KeyT, ValueT>& map) = delete;
	ManagedResourceMap<KeyT, ValueT>& operator=(ManagedResourceMap<KeyT, ValueT>&& map) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	template<typename... ArgsT>
	shared_ptr<ValueT> getOrCreateResource(const KeyT& key, ArgsT&&... args) noexcept;

	shared_ptr<ValueT> getResource(const KeyT& key) const noexcept;


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unordered_map<KeyT, weak_ptr<ValueT>> resource_map;
};


#include "managed_resource_map.tpp"