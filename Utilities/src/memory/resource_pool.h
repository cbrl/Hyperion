#pragma once

#include "memory/allocator/pool_allocator.h"
#include "exception/exception.h"


//----------------------------------------------------------------------------------
// IResourcePool
//----------------------------------------------------------------------------------
//
// Interface class for the ResourcePool.
//
//----------------------------------------------------------------------------------

class IResourcePool {
public:
	IResourcePool() = default;
	virtual ~IResourcePool() = default;

	// Allocate an unitialized object
	[[nodiscard]]
	virtual void* allocateObject() = 0;

	// Destroy and deallocate an object
	virtual void destroyObject(void* object) = 0;

	// Get the number of objects created
	[[nodiscard]]
	virtual size_t getCount() const = 0;
};


//----------------------------------------------------------------------------------
// ResourcePool
//----------------------------------------------------------------------------------
//
// The resource pool allocates memory for objects in chunks using the pool
// allocator. The size of each new chunk is twice that of the last (up to a maximum)
//
//               DataT: The datatype this pool holds
// InitialChunkObjects: The number of objects the initial chunk holds
//     MaxChunkObjects: The maximum size of the chunks
//
//----------------------------------------------------------------------------------

template<typename DataT, size_t InitialChunkObjects = 16, size_t MaxChunkObjects = 1024>
class ResourcePool : public IResourcePool {
private:
	struct Chunk {
	public:
		explicit Chunk(size_t pool_size) {
			allocator   = std::make_unique<PoolAllocator<DataT>>(pool_size);
			start_addr  = reinterpret_cast<uintptr>(allocator->getStartAddr());
			memory_size = pool_size;
		}

		~Chunk() {
			// Destroy the objects (allocator will free the memory upon deletion)
			for (auto* object : objects) {
				object->~DataT();
			}
			objects.clear();
		}

	public:
		unique_ptr<PoolAllocator<DataT>> allocator;
		std::list<DataT*> objects;

		uintptr start_addr;
		size_t memory_size;
	};


public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ResourcePool();
	ResourcePool(const ResourcePool& pool) = delete;
	ResourcePool(ResourcePool&& pool) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ResourcePool() = default;


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
	void* allocateObject() override;


	// Allocate and initialize an object
	template<typename... ArgsT>
	[[nodiscard]]
	DataT* constructObject(ArgsT&&... args);


	//----------------------------------------------------------------------------------
	// Member Functions -  Deallocation
	//----------------------------------------------------------------------------------

	// Destroy and deallocate an object
	void destroyObject(DataT* object);


	// Destroy and deallocate an object
	void destroyObject(void* object) override {
		destroyObject(static_cast<DataT*>(object));
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Miscellaneous
	//----------------------------------------------------------------------------------

	// Get the number of objects contained in this pool
	[[nodiscard]]
	size_t getCount() const override { return count; }


	// Apply an action to each resource
	template<typename ActionT>
	void forEach(ActionT act) {
		for (auto& chunk : memory_chunks) {
			for (auto* object : chunk->objects) {
				act(*object);
			}
		}
	}


private:
	unique_ptr<Chunk>& createChunk();


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::list<unique_ptr<Chunk>> memory_chunks;
	size_t chunk_objects = InitialChunkObjects;
	size_t count;
};

#include "resource_pool.tpp"




//----------------------------------------------------------------------------------
// ResourcePool Factory
//----------------------------------------------------------------------------------
//
// Creates a new resource pool for each unique resource type.
//
// Requirements:
//  - Resource class has a public static member 'index' of type std::type_index
//
// 'index' is used as an index into the map of resource pools. This requirement
// allows the user to retrieve the proper pool even when the specific resource
// type is unkown (e.g. when a polymorphic type is referred to by its base class).
//
//----------------------------------------------------------------------------------

class ResourcePoolFactory final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ResourcePoolFactory() = default;
	ResourcePoolFactory(const ResourcePoolFactory& factory) = delete;
	ResourcePoolFactory(ResourcePoolFactory&& factory) = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ResourcePoolFactory() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ResourcePoolFactory& operator=(const ResourcePoolFactory& factory) = delete;
	ResourcePoolFactory& operator=(ResourcePoolFactory&& factory) noexcept = default;


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
		const auto& it = pools.find(ResourceT::index);

		assert(it != pools.end() && "Invalid resource pool type requested");
		return static_cast<ResourcePool<ResourceT>*>(it->second.get());
	}


	// Get an existing pool using the resource index
	[[nodiscard]]
	IResourcePool* getPool(std::type_index type) {
		const auto& it = pools.find(type);

		assert(it != pools.end() && "Invalid resource pool type requested");
		return it->second.get();
	}


	// Check if a pool exists for the specified type
	template<typename ResourceT>
	bool poolExists() const {
		return pools.find(ResourceT::index) != pools.end();
	}


	// Check if a pool exists for the specified index
	bool poolExists(std::type_index type) const {
		return pools.find(type) != pools.end();
	}


private:
	std::unordered_map<std::type_index, unique_ptr<IResourcePool>> pools;
};
