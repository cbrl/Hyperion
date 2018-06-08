#pragma once

#include "datatypes/datatypes.h"
#include "allocator_util.h"


class Allocator {
public:
	Allocator(size_t memory_size);
	virtual ~Allocator();

	virtual void* allocate(const size_t size, const size_t alignment = 0) = 0;
	virtual void freeMemory(void* ptr) = 0;

	void* getStartAddr() const;


protected:
	void*  start_ptr = nullptr;
	size_t memory_size;
	size_t memory_used;
	size_t peak;
};
