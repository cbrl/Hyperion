#pragma once

#include "memory/allocator/pool_allocator.h"


//----------------------------------------------------------------------------------
// Resource Pool
//----------------------------------------------------------------------------------
//
// The resource pool allocates memory for objects in chunks using the pool
// allocator. The iterator provided allows the objects to be iterated over
// without worrying about the underlying chunks.
//
//----------------------------------------------------------------------------------

class IResourcePool {
public:
	virtual ~IResourcePool() = default;

	[[nodiscard]]
	virtual void* allocateObject() = 0;

	virtual void destroyObject(void* object) = 0;

	[[nodiscard]]
	virtual size_t getCount() const = 0;
};


template<typename DataT, size_t MaxObjsPerChunk = 128>
class ResourcePool : public IResourcePool {
private:
	struct Chunk {
		Chunk(PoolAllocator<DataT>* alloc) : allocator(alloc) {

			start_addr = reinterpret_cast<uintptr>(allocator->getStartAddr());
			memory_size = alloc_size;
		}

		PoolAllocator<DataT>* allocator;
		std::list<DataT*> objects;

		uintptr start_addr;
		size_t memory_size;
	};


public:
	ResourcePool();
	~ResourcePool();

	[[nodiscard]]
	void* allocateObject() override;

	void destroyObject(void* object) override;

	[[nodiscard]]
	size_t getCount() const override { return count; }

	// Apply an action to each resource
	template<typename ActionT>
	void forEach(ActionT act) {
		for (auto* chunk : memory_chunks) {
			for (auto* object : chunk->objects) {
				act(*object);
			}
		}
	}


private:
	std::list<Chunk*> memory_chunks;
	static constexpr size_t alloc_size = MaxObjsPerChunk * sizeof(DataT);
	size_t count;
};




//----------------------------------------------------------------------------------
// ResourcePool Factory
//----------------------------------------------------------------------------------
//
// Creates a new resource pool for each unique resource type.
//
// Requirements:
//  - Base resource class has static member 'type_id' of type std::type_index
//
// type_id is used as an index into the map of resource pools. This requirement
// allows the user to retrieve the proper pool even when the specific resource
// type is unkown (e.g. when a resource needs to be destroyed, but is referred
// to by its base class).
//
//----------------------------------------------------------------------------------

class ResourcePoolFactory final {
public:
	ResourcePoolFactory() = default;

	~ResourcePoolFactory() {
		for (auto& pair : pools) {
			delete pair.second;
			pair.second = nullptr;
		}
	}

	template<typename ResourceT>
	[[nodiscard]]
	ResourcePool<ResourceT>* getOrCreatePool() {

		using pool_t = ResourcePool<ResourceT>;

		const auto& it = pools.find(ResourceT::type_id);

		if (it == pools.end()) {
			pools[ResourceT::type_id] = new pool_t();
			return static_cast<pool_t*>(pools[ResourceT::type_id]);
		}

		return static_cast<pool_t*>(it->second);
	}

	template<typename ResourceT>
	[[nodiscard]]
	ResourcePool<ResourceT>* getPool() {
		const auto& it = pools.find(ResourceT::type_id);

		assert(it != pools.end() && "Invalid resource pool type requested");

		return static_cast<ResourcePool<ResourceT>*>(it->second);
	}

	[[nodiscard]]
	IResourcePool* getPool(type_index type) {
		const auto& it = pools.find(type);

		assert(it != pools.end() && "Invalid resource pool type requested");

		return it->second;
	}

	template<typename ResourceT>
	bool poolExists() const {
		return pools.find(ResourceT::type_id) != pools.end();
	}

	bool poolExists(type_index type) const {
		return pools.find(type) != pools.end();
	}


private:
	unordered_map<type_index, IResourcePool*> pools;
};


#include "resource_pool.tpp"
