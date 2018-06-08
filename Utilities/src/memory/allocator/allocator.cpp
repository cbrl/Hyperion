#include "allocator.h"


Allocator::Allocator(size_t memory_size)
	: memory_size(memory_size)
	, memory_used(0)
	, peak(0) {
}


Allocator::~Allocator() {
	memory_size = 0;
}


void* Allocator::getStartAddr() const {

	return start_ptr;
}
