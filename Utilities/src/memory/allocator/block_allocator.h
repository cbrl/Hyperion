#pragma once

#include "allocator_util.h"
#include "datatypes/datatypes.h"


template<typename T>
class BlockAllocator {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	BlockAllocator(size_t memory_size)
		: start_ptr(nullptr)
		, memory_size(memory_size)
		, memory_used(0) {

		start_ptr = malloc(memory_size);
	}

	BlockAllocator(const BlockAllocator& allocator) = delete;
	BlockAllocator(BlockAllocator&& allocator) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~BlockAllocator() {
		free(start_ptr);
		start_ptr = nullptr;
	}


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	BlockAllocator& operator=(const BlockAllocator& allocator) = delete;
	BlockAllocator& operator=(BlockAllocator&& allocator) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Reset the state of the allocator
	virtual void reset() = 0;

	// Allocate memory for an object
	virtual T* allocate() = 0;

	// Deallocate an object
	virtual void deallocate(T* ptr) = 0;

	// Get the beginning of the memory block
	void* getStartAddr() const {
		return start_ptr;
	}


protected:
	void* start_ptr;
	size_t memory_size;
	size_t memory_used;
};