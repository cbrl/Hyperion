#pragma once

#include "block_allocator.h"


class LinearAllocator final : public BlockAllocator<void> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	LinearAllocator(size_t memory_size);
	LinearAllocator(const LinearAllocator& allocator) = delete;
	LinearAllocator(LinearAllocator&& allocator) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~LinearAllocator() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	LinearAllocator& operator=(const LinearAllocator& allocator) = delete;
	LinearAllocator& operator=(LinearAllocator&& allocator) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void reset() override;
	void deallocate(void* ptr) override;

	void* allocate() override {
		static_assert(true, "Calling LinearAllocator::allocate() without template parameter");
		return nullptr;
	}

	template<typename T>
	void* allocate() {
		size_t padding = 0;

		const uintptr curr_addr = reinterpret_cast<uintptr>(start_ptr) + this->offset;

		// Calculate the padding if it doesn't fit nicely
		if ((alignof(T) != 0) && (offset % alignof(T) != 0)) {
			padding = AllocatorUtil::CalculatePadding(curr_addr, alignof(T));
		}

		// Check if enough memory is available
		if (memory_used + sizeof(T) + padding > memory_size) {
			return nullptr;
		}

		this->offset += (padding + sizeof(T));
		this->memory_used = offset;

		const uintptr next_addr = curr_addr + padding;
		return reinterpret_cast<void*>(next_addr);
	}



protected:
	size_t offset;
};
