#include "stdafx.h"
#include "linear_allocator.h"


LinearAllocator::LinearAllocator(const size_t memory_size)
	: Allocator(memory_size) {
}


LinearAllocator::~LinearAllocator() {
	Free(start_ptr);
	start_ptr = nullptr;
}


void LinearAllocator::Init() {

	if (start_ptr == nullptr) {
		start_ptr = malloc(memory_size);
		offset = 0;
	}
	else {
		free(start_ptr);
	}
}


void LinearAllocator::Reset() {
	offset = 0;
	memory_used = 0;
	peak = 0;
}


void* LinearAllocator::Allocate(const size_t size, const size_t alignment) {

	size_t padding     = 0;
	uptr   padded_addr = 0;

	const uptr curr_addr = (uptr)start_ptr + this->offset;

	// Calculate the padding if it doesn't fit nicely
	if ((alignment != 0) && (offset % alignment != 0)) {
		AllocatorUtil::CalculatePadding(curr_addr, alignment);
	}

	// Check if enough memory is available
	if (memory_used + size + padding > memory_size) {
		return nullptr;
	}


	this->offset += (padding + size);

	this->memory_used = offset;
	this->peak = std::max(peak, memory_used);

	const uptr next_addr = curr_addr + padding;

	return (void*)next_addr;
}


void LinearAllocator::Free(void* ptr) {

	assert(false && "Linear allocator doesn't support free(). Use the reset() method.");
}
