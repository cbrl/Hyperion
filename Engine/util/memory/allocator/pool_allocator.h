#pragma once

#include "allocator.h"


template<typename T>
class LinkedList {
	public:
		struct Node {
			T     data;
			Node* next = nullptr;
		};


	public:
		Node * first;


	public:
		LinkedList() = default;

		void push_front(Node* new_node) {
			new_node->next = first;
			first = new_node;
		}

		Node* pop_front() {
			Node* front = first;
			first = front->next;
			return front;
		}
};


class PoolAllocator : public Allocator {
	public:
		PoolAllocator(const size_t memory_size, const size_t chunk_size);
		virtual ~PoolAllocator();

		virtual void  Init() override;
		virtual void  Reset();
		virtual void* Allocate(const size_t size, const size_t alignment = 0) override;
		virtual void  Free(void* ptr);


	private:
		LinkedList<std::monostate> free_list;
		using Node = LinkedList<std::monostate>::Node;


	protected:
		void*  start_ptr = nullptr;
		size_t chunk_size;
};
