#pragma once

#include "util\datatypes\datatypes.h"
#include "allocator_util.h"


class Allocator {
	public:
		Allocator(const size_t memory_size);
		virtual ~Allocator();

		virtual void  Init() = 0;
		virtual void* Allocate(const size_t size, const size_t alignment = 0) = 0;
		virtual void  Free(void* ptr) = 0;


	protected:
		size_t memory_size;
		size_t memory_used;
		size_t peak;
};