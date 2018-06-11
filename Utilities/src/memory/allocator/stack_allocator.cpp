#include "stack_allocator.h"


StackAllocator::StackAllocator(const size_t memory_size)
	: Allocator(memory_size) {

	if (start_ptr == nullptr) {
		start_ptr = malloc(memory_size);
		offset = 0;
	}
	else {
		freeMemory(start_ptr);
	}
}


StackAllocator::~StackAllocator() {
	free(start_ptr);
	start_ptr = nullptr;
}


void StackAllocator::reset() {
	memory_used = 0;
	offset = 0;
	peak = 0;
}


void* StackAllocator::allocate(const size_t size, const size_t alignment) {

	// Ensure the the allocation size and start pointer are valid
	assert(size > 0 && "StackAllocator::Allocate() called with 0 size.");

	const uintptr curr_addr = reinterpret_cast<uintptr>(start_ptr) + this->offset;

	const size_t padding = AllocatorUtil::CalculatePadding(curr_addr, alignment, sizeof(AllocHeader));

	// Check if enough memory is available
	if (memory_used + size + padding > memory_size) {
		return nullptr;
	}


	this->offset += (padding + size);
	this->memory_used = offset;
	this->peak = std::max(peak, memory_used);


	const uintptr next_addr = curr_addr + padding;
	const uintptr header_addr = next_addr - sizeof(AllocHeader);

	// Store padding amount in the header
	reinterpret_cast<AllocHeader*>(header_addr)->padding = static_cast<u8>(padding);

	return reinterpret_cast<void*>(next_addr);
}


void StackAllocator::freeMemory(void* ptr) {

	const uintptr curr_addr = reinterpret_cast<uintptr>(ptr);
	const uintptr header_addr = curr_addr - sizeof(AllocHeader);

	const AllocHeader* header = reinterpret_cast<AllocHeader*>(header_addr);

	this->offset = curr_addr - reinterpret_cast<uintptr>(this->start_ptr) - header->padding;
	this->memory_used = offset;
}