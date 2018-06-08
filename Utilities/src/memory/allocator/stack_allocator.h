#pragma once

#include "allocator.h"


class StackAllocator : public Allocator {
public:
	StackAllocator(size_t memory_size);
	virtual ~StackAllocator();

	virtual void reset();
	void* allocate(size_t size, size_t alignment = 0) override;
	void freeMemory(void* ptr) override;


private:
	struct AllocHeader {
		u8 padding;
	};


protected:
	size_t offset;
};
