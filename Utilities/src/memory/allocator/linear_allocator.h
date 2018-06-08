#pragma once

#include "allocator.h"


class LinearAllocator : public Allocator {
public:
	LinearAllocator(size_t memory_size);
	virtual ~LinearAllocator();

	virtual void reset();
	void* allocate(size_t size, size_t alignment = 0) override;
	void freeMemory(void* ptr) override;


protected:
	size_t offset;
};
