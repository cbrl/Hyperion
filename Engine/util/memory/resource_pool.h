#pragma once

#include "util\memory\allocator\pool_allocator.h"


template<typename DataT, size_t max_objs_per_chunk = 512>
class ResourcePool {

	static const size_t alloc_size = max_objs_per_chunk * sizeof(DataT);

	protected:
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
};


#include "resource_pool.tpp"