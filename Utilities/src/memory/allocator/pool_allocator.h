#pragma once

#include "allocator.h"


template<typename DataT>
class PoolAllocator : public Allocator {

protected:
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


public:
	PoolAllocator(size_t memory_size, size_t chunk_size = sizeof(DataT), size_t alignment = alignof(DataT));
	virtual ~PoolAllocator();

	virtual void reset();
	void* allocate(size_t size = sizeof(DataT), size_t alignment = alignof(DataT)) override;
	virtual DataT* allocateCast();
	void freeMemory(void* ptr) override;
	void freeMemory(DataT* ptr);


private:
	EmptyLinkedList free_list;
	using node = typename EmptyLinkedList::Node;


protected:
	const size_t chunk_size;
	const size_t align;


public:
	using value_type = DataT;
};


#include "pool_allocator.tpp"
