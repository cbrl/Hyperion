#pragma once

#include "datatypes/datatypes.h"
#include "exception/exception.h"
#include "log/log.h"
#include <memory_resource>


//----------------------------------------------------------------------------------
// ResourcePool
//----------------------------------------------------------------------------------
//
// Constructs resources of type T using a std::list with the
// std::pmr::synchronized_pool_resource memory resource.
//
//----------------------------------------------------------------------------------

class IResourcePool {
public:
	virtual ~IResourcePool() = default;

	// Remove a resource referenced to by its pointer
	virtual void remove_resource(void* resource) = 0;

protected:
	std::pmr::synchronized_pool_resource pool_resource;
};

template <typename T>
class ResourcePool final : public IResourcePool, public std::pmr::list<T> {
public:
	ResourcePool() : std::pmr::list<T>(&pool_resource) {}

	// Remove a resource referenced to by its pointer
	void remove_resource(void* resource) {
		this->remove_if([resource](T& element) { return &element == resource; });
	}
};




//----------------------------------------------------------------------------------
// ResourcePool Manager
//----------------------------------------------------------------------------------
//
// Creates a new resource pool for each unique resource type.
//
// Requirements:
//  - Resource class has a public static member 'index' of type std::type_index
//
// 'index' is used as an index into the map of resource pools. This requirement
// allows the user to retrieve the proper pool even when the specific resource
// type is unknown (e.g. using a virtual getTypeIndex() method used when a
// polymorphic type is referred to by its base class).
//
//----------------------------------------------------------------------------------

class ResourcePoolManager final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ResourcePoolManager() = default;
	ResourcePoolManager(const ResourcePoolManager& factory) = delete;
	ResourcePoolManager(ResourcePoolManager&& factory) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ResourcePoolManager() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ResourcePoolManager& operator=(const ResourcePoolManager& factory) = delete;
	ResourcePoolManager& operator=(ResourcePoolManager&& factory) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Get an existing pool or create it if it doesn't exist
	template <typename ResourceT>
	[[nodiscard]]
	ResourcePool<ResourceT>* getOrCreatePool() {
		using pool_t = ResourcePool<ResourceT>;
		const auto pair = pools.try_emplace(ResourceT::index, std::make_unique<pool_t>()); //pair = std::pair<iterator, bool>
		return static_cast<pool_t*>(pair.first->second.get());
	}


	// Get an existing pool using the resource type
	template <typename ResourceT>
	[[nodiscard]]
	ResourcePool<ResourceT>* getPool() {
		auto* pool = getPool(ResourceT::index);
		if (pool) {
			return static_cast<ResourcePool<ResourceT>*>(getPool(ResourceT::index));
		}
		else {
			Logger::log(LogLevel::err, "ResourcePoolManager::getPool<{}>() - Invalid pool type requested", type_name<ResourceT>());
			return nullptr;
		}
	}


	// Get an existing pool using the resource index
	[[nodiscard]]
	IResourcePool* getPool(std::type_index type) {
		const auto& it = pools.find(type);
		if (it == pools.end()) {
			Logger::log(LogLevel::err, "ResourcePoolManager::getPool<{}>() - Invalid pool type requested", type.name());
			assert(false && "ResourcePoolManager::getPool() - Invalid resource pool type requested");
			return nullptr;
		}
		return it->second.get();
	}


	// Check if a pool exists for the specified type
	template <typename ResourceT>
	bool poolExists() const noexcept {
		return poolExists(ResourceT::index);
	}


	// Check if a pool exists for the specified index
	bool poolExists(std::type_index type) const noexcept {
		return pools.find(type) != pools.end();
	}


private:
	std::unordered_map<std::type_index, std::unique_ptr<IResourcePool>> pools;
};
