#include "stack_allocator.h"
#include "exception/exception.h"

StackAllocator::StackAllocator(const size_t memory_size)
	: BlockAllocator(memory_size)
	, offset(0) {
}


void StackAllocator::reset() {
	memory_used = 0;
	offset = 0;
}


void StackAllocator::deallocate(void* ptr) {

	const uintptr curr_addr = reinterpret_cast<uintptr>(ptr);

	assert(curr_addr >= reinterpret_cast<uintptr>(this->start_ptr) &&
	       curr_addr <= (reinterpret_cast<uintptr>(this->start_ptr) + this->memory_size) &&
		   "PoolAllocator::deallocate() - Invalid address specified for deallocation");

	
	const uintptr header_addr = curr_addr - sizeof(AllocHeader);
	const AllocHeader* header = reinterpret_cast<AllocHeader*>(header_addr);

	this->offset = curr_addr - reinterpret_cast<uintptr>(this->start_ptr) - header->padding;
	this->memory_used = offset;
}
