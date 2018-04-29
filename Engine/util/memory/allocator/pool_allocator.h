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
				Node* first;

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
		PoolAllocator(const size_t memory_size, const size_t chunk_size = sizeof(DataT), const size_t alignment = alignof(DataT));
		virtual ~PoolAllocator();

		virtual void   Init() override;
		virtual void   Reset();
		virtual void*  Allocate(const size_t size = sizeof(DataT), const size_t alignment = alignof(DataT)) override;
		virtual DataT* AllocateCast();
		virtual void   Free(void* ptr) override;
		virtual void   Free(DataT* ptr);


	private:
		EmptyLinkedList free_list;
		using Node = typename EmptyLinkedList::Node;


	protected:
		const size_t chunk_size;
		const size_t align;


	public:
		using value_type = DataT;
};


#include "pool_allocator.tpp"