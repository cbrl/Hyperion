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
			Chunk(PoolAllocator<DataT>* alloc) : allocator(alloc) {

				start_addr  = allocator->GetStartAddr();
				memory_size = alloc_size;
			}

			PoolAllocator<DataT>* allocator;
			std::list<DataT*> objects;

			uintptr start_addr;
			size_t  memory_size;
		};


	public:
		class iterator : public std::iterator<std::forward_iterator_tag, DataT> {
			using chunk_iter = typename std::list<Chunk*>::iterator;
			using object_iter = typename std::list<DataT*>::iterator;

			public:
			iterator(chunk_iter begin, chunk_iter end)
				: chunk_current(begin)
				, chunk_end(end) {

				if (begin != end) {
					assert(*chunk_current != nullptr && "ResourcePool - Invalid iterator");
					object_current = (*chunk_current)->objects.begin();
				}
			}

			iterator& operator++() {

				object_current++;

				if (object_current == (*chunk_current)->objects.end()) {
					chunk_current++;

					if (chunk_current != chunk_end) {
						assert(*chunk_current != nullptr && "ResourcePool - Invalid iterator");
						object_current = (*chunk_current)->objects.begin();
					}
				}

				return *this;
			}

			DataT& operator*()  const { return object_current.operator*(); }
			DataT* operator->() const { return object_current.operator->(); }


			private:
			chunk_iter  chunk_current;
			chunk_iter  chunk_end;
			object_iter object_current;
		};


	public:
		ResourcePool();
		~ResourcePool();

		void* CreateObject() override;
		void  DestroyObject(void* object) override;

		iterator begin() { return iterator(memory_chunks.begin(), memory_chunks.end()); }
		iterator end()   { return iterator(memory_chunks.end(),   memory_chunks.end()); }


	private:
		std::list<Chunk*> memory_chunks;
		static constexpr size_t alloc_size = max_objs_per_chunk * sizeof(DataT);
};



//----------------------------------------------------------------------------------
// ResourcePool Map
//----------------------------------------------------------------------------------
//
// Creates a unique resource pool for each type of resource.
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

			const auto& it = pools.find(ResourceT::type_id);

			if (it == pools.end()) {
				pools[ResourceT::type_id] = new pool_t();
				return static_cast<pool_t*>(pools[ResourceT::type_id]);
			}

			return static_cast<pool_t*>(it->second);
		}

		template<typename ResourceT>
		ResourcePool<ResourceT>* GetPool() {
			const auto& it = pools.find(ResourceT::type_id);

			assert(it != pools.end() && "Invalid resource pool type requested");

			return static_cast<ResourcePool<ResourceT>*>(it->second);
		}

		IResourcePool* GetPool(type_index type) {
			const auto& it = pools.find(type);

			assert(it != pools.end() && "Invalid resource pool type requested");

			return it->second;
		}


	private:
		unordered_map<type_index, IResourcePool*> pools;
};


#include "resource_pool.tpp"