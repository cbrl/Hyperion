#pragma once

#include "allocator.h"


class LinearAllocator : public Allocator {
	public:
		LinearAllocator(const size_t memory_size);
		virtual ~LinearAllocator();

		virtual void  Init() override;
		virtual void  Reset();
		virtual void* Allocate(const size_t size, const size_t alignment = 0) override;
		virtual void  Free(void* ptr) override;


	protected:
		void*  start_ptr = nullptr;
		size_t offset;
};