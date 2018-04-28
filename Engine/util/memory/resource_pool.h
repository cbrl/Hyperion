#pragma once

#include "util\memory\allocator\pool_allocator.h"


class IResourcePool {
	public:
		virtual ~IResourcePool() = default;
		virtual void* CreateObject() = 0;
		virtual void  DestroyObject(void* object) = 0;
};


template<typename DataT, size_t max_objs_per_chunk = 512>
class ResourcePool : public IResourcePool {
	private:
		struct Chunk {
			Chunk(PoolAllocator* alloc) : allocator(alloc) {

				start_addr  = allocator->GetStartAddr();
				memory_size = alloc_size;
			}

			PoolAllocator*    allocator;
			std::list<DataT*> objects;

			uintptr start_addr;
			size_t  memory_size;
		};


	public:
		ResourcePool();
		~ResourcePool();

		void* CreateObject();
		void  DestroyObject(void* object);


	private:
		std::list<Chunk*> memory_chunks;

		static const size_t alloc_size = max_objs_per_chunk * sizeof(DataT);
};



//----------------------------------------------------------------------------------
// ResourcePool Map
//----------------------------------------------------------------------------------
//
// Creates a unique resource pool for each type of resource. Resources should derive
// from a base class with a static member 'type_id' of type std::type_index.
// This is used as an index into the map. It is kept as member so the proper
// pool can be retrieved even when the specific resource type is unkown (e.g. when
// a resource needs to be deleted, but is referred to by its base class).
//
//----------------------------------------------------------------------------------

class ResourcePoolMap final {
	public:
		ResourcePoolMap() = default;

		~ResourcePoolMap() {
			for (auto& pair : pools) {
				delete pair.second;
				pair.second = nullptr;
			}
		}

		template<typename ResourceT>
		ResourcePool<ResourceT>* GetOrCreatePool() {

			using pool_t = ResourcePool<ResourceT>;

			const auto it = pools.find(ResourceT::type_id);

			if (it == pools.end()) {
				pools[ResourceT::type_id] = new pool_t();
				return static_cast<pool_t*>(pools[ResourceT::type_id]);
			}

			return static_cast<pool_t*>(it->second);
		}

		template<typename ResourceT>
		ResourcePool<ResourceT>* GetPool() {
			const auto it = pools.find(ResourceT::type_id);

			assert(it != pools.end() && "Invalid resource pool type requested");

			return static_cast<ResourcePool<ResourceT>*>(it->second);
		}

		IResourcePool* GetPool(type_index type) {
			const auto it = pools.find(type);

			assert(it != pools.end() && "Invalid resource pool type requested");

			return it->second;
		}


	private:
		unordered_map<type_index, IResourcePool*> pools;
};


#include "resource_pool.tpp"