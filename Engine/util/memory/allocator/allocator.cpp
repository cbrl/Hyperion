#include "stdafx.h"
#include "allocator.h"


Allocator::Allocator(const size_t memory_size)
	: memory_size(memory_size)
	, memory_used(0) {
}


Allocator::~Allocator() {
	memory_size = 0;
}


void* Allocator::GetStartAddr() {

	return start_ptr;
}