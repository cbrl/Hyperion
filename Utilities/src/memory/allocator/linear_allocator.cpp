#include "linear_allocator.h"
#include "exception/exception.h"


LinearAllocator::LinearAllocator(size_t memory_size)
	: BlockAllocator(memory_size)
	, offset(0) {
}


void LinearAllocator::reset() {
	memory_used = 0;
	offset = 0;
}


void LinearAllocator::deallocate(void* ptr) {

	assert(false && "Linear allocator doesn't support deallocate(). Use the reset() method.");
}
