#pragma once

#include "block_allocator.h"
#include "exception/exception.h"


class EmptyLinkedList {
public:
	struct Node {
		Node* next = nullptr;
	};

public:
	EmptyLinkedList() = default;

	void push_front(Node* new_node) {
		new_node->next = first;
		first = new_node;
	}

	Node* pop_front() {
		Node* front = first;
		first = first->next;
		return front;
	}

public:
	Node* first;
};


template<typename T>
class PoolAllocator final : public BlockAllocator<T> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	PoolAllocator(size_t memory_size);
	PoolAllocator(const PoolAllocator& allocator) = delete;
	PoolAllocator(PoolAllocator&& allocator) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~PoolAllocator() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	PoolAllocator& operator=(const PoolAllocator& allocator) = delete;
	PoolAllocator& operator=(PoolAllocator&& allocator) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void reset() override;
	T* allocate() override;
	void deallocate(T* ptr) override;


private:
	EmptyLinkedList free_list;
	using node = typename EmptyLinkedList::Node;


public:
	using value_type = T;
};


#include "pool_allocator.tpp"
