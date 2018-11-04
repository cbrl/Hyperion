#pragma once

#include "datatypes/datatypes.h"
#include "exception/exception.h"
#include "log/log.h"
#include <memory_resource>


//----------------------------------------------------------------------------------
// IResourcePool
//----------------------------------------------------------------------------------
//
// Interface class for the ResourcePool.
//
//----------------------------------------------------------------------------------

template<typename T = void>
class PoolResource {
protected:
	static std::pmr::synchronized_pool_resource pool;
};

template<typename T>
std::pmr::synchronized_pool_resource PoolResource<T>::pool;


class IResourcePool : public PoolResource<> {
public:
	IResourcePool() noexcept = default;
	virtual ~IResourcePool() = default;

	// Allocate an uninitialized object
	[[nodiscard]]
	virtual void* allocate() = 0;

	// Destroy and deallocate an object
	virtual void deallocate(void* object) = 0;

	// Get the number of objects created
	[[nodiscard]]
	virtual size_t getCount() const noexcept = 0;
};




//----------------------------------------------------------------------------------
// ResourcePool
//----------------------------------------------------------------------------------
//
// Constructs objects using std::pmr::synchronized_pool_resource
// as the underlying memory allocator.
//
//----------------------------------------------------------------------------------

template<typename T>
class ResourcePool final : public IResourcePool {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ResourcePool() = default;
	ResourcePool(const ResourcePool& pool) = delete;
	ResourcePool(ResourcePool&& pool) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ResourcePool();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ResourcePool& operator=(const ResourcePool& pool) = delete;
	ResourcePool& operator=(ResourcePool&& pool) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Allocation
	//----------------------------------------------------------------------------------

	// Allocate memory for an object
	[[nodiscard]]
	void* allocate() override;

	// Allocate and initialize an object
	template<typename... ArgsT>
	[[nodiscard]]
	T* construct(ArgsT&&... args);


	//----------------------------------------------------------------------------------
	// Member Functions -  Deallocation
	//----------------------------------------------------------------------------------

	// Destroy and deallocate an object
	void deallocate(T* object);

	void deallocate(void* object) override {
		deallocate(static_cast<T*>(object));
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Miscellaneous
	//----------------------------------------------------------------------------------

	// Get the number of objects contained in this pool
	[[nodiscard]]
	size_t getCount() const noexcept override {
		return objects.size();
	}


	// Apply an action to each resource
	template<typename ActionT>
	void forEach(ActionT&& act) {
		for (auto* obj : objects) {
			act(*obj);
		}
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::list<T*> objects;
};
#include "resource_pool.tpp"




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
	ResourcePoolManager(ResourcePoolManager&& factory) = default;

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
	template<typename ResourceT>
	[[nodiscard]]
	ResourcePool<ResourceT>* getOrCreatePool() {
		using pool_t = ResourcePool<ResourceT>;
		// pair = std::pair<iterator, bool>
		const auto pair = pools.try_emplace(ResourceT::index, std::make_unique<pool_t>());
		return static_cast<pool_t*>(pair.first->second.get());
	}


	// Get an existing pool using the resource type
	template<typename ResourceT>
	[[nodiscard]]
	ResourcePool<ResourceT>* getPool() {
		return static_cast<ResourcePool<ResourceT>*>(getPool(ResourceT::index));
	}


	// Get an existing pool using the resource index
	[[nodiscard]]
	IResourcePool* getPool(std::type_index type) {
		const auto& it = pools.find(type);
		if (it == pools.end()) {
			Logger::log(LogLevel::err, "Invalid pool type requested: {}", type.name());
			assert(false && "Invalid resource pool type requested");
			return nullptr;
		}
		return it->second.get();
	}


	// Check if a pool exists for the specified type
	template<typename ResourceT>
	bool poolExists() const {
		return poolExists(ResourceT::index);
	}


	// Check if a pool exists for the specified index
	bool poolExists(std::type_index type) const {
		return pools.find(type) != pools.end();
	}


private:
	std::unordered_map<std::type_index, unique_ptr<IResourcePool>> pools;
};
