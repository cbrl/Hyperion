#pragma once

#include "allocator.h"


class PoolAllocator : public Allocator {

	protected:
		class EmptyLinkedList {
			public:
				struct Node {
					Node* next = nullptr;
				};

			public:
				Node * first;

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
		};


	public:
		PoolAllocator(const size_t memory_size, const size_t chunk_size, const size_t alignment);
		virtual ~PoolAllocator();

		virtual void  Init() override;
		virtual void  Reset();
		virtual void* Allocate(const size_t size, const size_t alignment = 0) override;
		virtual void  Free(void* ptr);


	private:
		EmptyLinkedList free_list;
		using Node = EmptyLinkedList::Node;


	protected:
		const size_t chunk_size;
		const size_t align;
};
