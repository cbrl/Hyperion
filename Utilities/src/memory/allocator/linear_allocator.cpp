#include "linear_allocator.h"


LinearAllocator::LinearAllocator(const size_t memory_size)
	: Allocator(memory_size) {

	if (start_ptr == nullptr) {
		start_ptr = malloc(memory_size);
		offset = 0;
	}
	else {
		freeMemory(start_ptr);
	}
}


LinearAllocator::~LinearAllocator() {
	free(start_ptr);
	start_ptr = nullptr;
}


void LinearAllocator::reset() {
	memory_used = 0;
	offset = 0;
	peak = 0;
}


void* LinearAllocator::allocate(const size_t size, const size_t alignment) {

	// Ensure the allocation size is valid
	assert(size > 0 && "LinearAllocator::Allocate() called with 0 size.");

	size_t padding = 0;
	uintptr padded_addr = 0;

	const uintptr curr_addr = reinterpret_cast<uintptr>(start_ptr) + this->offset;

	// Calculate the padding if it doesn't fit nicely
	if ((alignment != 0) && (offset % alignment != 0)) {
		padding = AllocatorUtil::CalculatePadding(curr_addr, alignment);
	}

	// Check if enough memory is available
	if (memory_used + size + padding > memory_size) {
		return nullptr;
	}


	this->offset += (padding + size);
	this->memory_used = offset;
	this->peak = std::max(peak, memory_used);


	const uintptr next_addr = curr_addr + padding;

	return reinterpret_cast<void*>(next_addr);
}


void LinearAllocator::freeMemory(void* ptr) {

	assert(false && "Linear allocator doesn't support free(). Use the reset() method.");
}
