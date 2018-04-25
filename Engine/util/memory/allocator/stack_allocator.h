#pragma once

#include "allocator.h"


class StackAllocator : public Allocator {
	public:
		StackAllocator(const size_t memory_size);
		virtual ~StackAllocator();

		virtual void  Init() override;
		virtual void  Reset();
		virtual void* Allocate(const size_t size, const size_t alignment = 0) override;
		virtual void  Free(void* ptr);


	private:
		struct AllocHeader {
			u8 padding;
		};


	protected:
		void*  start_ptr = nullptr;
		size_t offset;
};