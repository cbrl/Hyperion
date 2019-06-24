#pragma once

#include "memory/handle/handle.h"
#include "memory/handle/handle_table.h"
#include "memory/resource_pool.h"

template<typename HandleT, typename ResourceT>
class HandleMap final {
	using resource_pool_type     = ResourcePool<typename HandleT::value_type, ResourceT>;

public:

	using handle_type            = HandleT;
	using value_type             = ResourceT;
	using pointer                = ResourceT *;
	using const_pointer          = const ResourceT*;
	using reference              = ResourceT &;
	using const_reference        = const ResourceT &;
	using size_type              = size_t;
	using difference_type        = ptrdiff_t;
	using iterator               = typename resource_pool_type::iterator;
	using const_iterator         = typename resource_pool_type::const_iterator;

	//----------------------------------------------------------------------------------
	// Member Functions - Modifiers
	//----------------------------------------------------------------------------------
	template<typename... ArgsT>
	[[nodiscard]]
	std::pair<handle_type, std::reference_wrapper<value_type>> create(ArgsT&& ... args) {
		auto handle = handle_table.createHandle();
		auto& resource = resource_pool.construct(handle.index, std::forward<ArgsT>(args)...);
		return { handle, std::ref(resource) };
	}

	void release(handle_type handle) {
		assert(contains(handle));
		resource_pool.erase(handle.index);
		handle_table.releaseHandle(handle);
	}

	void clear() noexcept {
		resource_pool.clear();
		handle_table.clear();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Access
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(handle_type handle) const noexcept {
		return handle_table.valid(handle);
	}

	[[nodiscard]]
	reference get(handle_type handle) {
		assert(contains(handle));
		return resource_pool.get(handle.index);
	}

	[[nodiscard]]
	const_reference get(handle_type handle) const {
		assert(contains(handle));
		return resource_pool.get(handle.index);
	}

	[[nodiscard]]
	pointer data() {
		return resource_pool.data();
	}

	[[nodiscard]]
	const_pointer data() const noexcept {
		return resource_pool.data();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iterators
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	iterator begin() {
		return resource_pool.begin();
	}

	[[nodiscard]]
	const_iterator begin() const {
		return resource_pool.begin();
	}

	[[nodiscard]]
	const_iterator cbegin() const {
		return resource_pool.cbegin();
	}

	[[nodiscard]]
	iterator end() {
		return resource_pool.end();
	}

	[[nodiscard]]
	const_iterator end() const {
		return resource_pool.end();
	}

	[[nodiscard]]
	const_iterator cend() const {
		return resource_pool.cend();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Capacity
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool empty() const {
		return resource_pool.empty();
	}

	[[nodiscard]]
	size_type size() const {
		return resource_pool.size();
	}

	void reserve(size_type new_cap) {
		resource_pool.reserve(new_cap);
		handle_table.reserve(new_cap);
	}

	void shrink_to_fit() {
		resource_pool.shrink_to_fit();
		handle_table.shrink_to_fit();
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	HandleTable<HandleT> handle_table;
	resource_pool_type   resource_pool;
};