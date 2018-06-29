#pragma once

#include "block_allocator.h"


class StackAllocator final : public BlockAllocator<void> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	StackAllocator(size_t memory_size);
	StackAllocator(const StackAllocator& allocator) = delete;
	StackAllocator(StackAllocator&& allocator) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~StackAllocator() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	StackAllocator& operator=(const StackAllocator& allocator) = delete;
	StackAllocator& operator=(StackAllocator&& allocator) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void reset() override;

	void deallocate(void* ptr) override;

	template<typename T>
	void deallocate(T* ptr) {
		deallocate(reinterpret_cast<void*>(ptr));
	}

	void* allocate() override {
		static_assert(true, "StackAllocator::allocate() called without template parameter");
		return nullptr;
	}

	template<typename T>
	void* allocate() {
		const uintptr curr_addr = reinterpret_cast<uintptr>(start_ptr) + this->offset;
		const size_t  padding   = AllocatorUtil::CalculatePadding(curr_addr, alignof(T), sizeof(AllocHeader));

		// Check if enough memory is available
		if (memory_used + sizeof(T) + padding > memory_size) {
			return nullptr;
		}

		this->offset += (padding + sizeof(T));
		this->memory_used = offset;

		const uintptr next_addr   = curr_addr + padding;
		const uintptr header_addr = next_addr - sizeof(AllocHeader);

		// Store padding amount in the header
		reinterpret_cast<AllocHeader*>(header_addr)->padding = static_cast<uint8_t>(padding);

		return reinterpret_cast<void*>(next_addr);
	}


private:
	struct AllocHeader {
		uint8_t padding;
	};


protected:
	size_t offset;
};
