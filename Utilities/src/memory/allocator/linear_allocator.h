#pragma once

#include "allocator.h"


class LinearAllocator final : public Allocator {
public:
	LinearAllocator(size_t memory_size);
	~LinearAllocator();

	void reset();
	void* allocate(size_t size, size_t alignment) override;
	void freeMemory(void* ptr) override;


protected:
	size_t offset;
};
