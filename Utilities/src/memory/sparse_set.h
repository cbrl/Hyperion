#pragma once

#include <vector>
#include <type_traits>
#include <assert.h>


template<typename T>
class SparseSet {
	static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>,
	              "Sparse sets only support unsigned integral types");

public:

	using container_type         = std::vector<T>;
	using value_type             = T;
	using pointer                = T*;
	using const_pointer          = const T*;
	using reference              = T&;
	using const_reference        = const T&;
	using size_type              = size_t;
	using difference_type        = ptrdiff_t;

	// The underlying dense container is reverse iterated. This ensures that the current
	// element can be deleted while iterating and no other elements will be skipped.
	// However, elements added while iterating will not be covered.
	using iterator               = typename container_type::reverse_iterator;
	using const_iterator         = typename container_type::const_reverse_iterator;


	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SparseSet() noexcept = default;
	SparseSet(const SparseSet&) noexcept = default;
	SparseSet(SparseSet&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~SparseSet() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SparseSet& operator=(const SparseSet&) noexcept = default;
	SparseSet& operator=(SparseSet&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Access
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(value_type val) const noexcept {
		return val < capacity()     &&
		       sparse[val] < size() &&
		       dense[sparse[val]]   == val;
	}

	[[nodiscard]]
	size_type index_of(value_type val) const noexcept {
		assert(contains(val));
		return sparse[val];
	}

	[[nodiscard]]
	pointer data() noexcept {
		return dense.data();
	}

	[[nodiscard]]
	const_pointer data() const noexcept {
		return dense.data();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iterators
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	iterator begin() noexcept {
		return dense.rbegin();
	}

	[[nodiscard]]
	const_iterator begin() const noexcept {
		return dense.rbegin();
	}

	[[nodiscard]]
	const_iterator cbegin() const noexcept {
		return dense.crbegin();
	}

	[[nodiscard]]
	iterator end() noexcept {
		return dense.rend();
	}

	[[nodiscard]]
	const_iterator end() const noexcept {
		return dense.rend();
	}

	[[nodiscard]]
	const_iterator cend() const noexcept {
		return dense.crend();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Capacity
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool empty() const noexcept {
		return dense.empty();
	}

	[[nodiscard]]
	size_type size() const noexcept {
		return dense.size();
	}

	[[nodiscard]]
	size_type capacity() const noexcept {
		return sparse.size();
	}

	virtual void reserve(size_type new_cap) {
		if (new_cap > sparse.size()) {
			dense.reserve(new_cap);
			sparse.resize(new_cap, 0);
		}
	}

	virtual void shrink_to_fit() {
		if (dense.empty()) {
			sparse.clear();
		}

		dense.shrink_to_fit();
		sparse.shrink_to_fit();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Modifiers
	//----------------------------------------------------------------------------------
	virtual void clear() noexcept {
		dense.clear();
		sparse.clear();
	}

	// Insert the given value into the sparse set. If the set's capacity is less than
	// the value, then it will be resized to the value + 1;
	virtual void insert(value_type val) {
		if (!contains(val)) {
			if (val >= sparse.size())
				reserve(val + 1);

			sparse[val] = dense.size();
			dense.push_back(val);
		}
	}

	virtual void erase(value_type val) {
		if (contains(val)) {
			dense[sparse[val]] = dense.back();
			sparse[dense.size() - 1] = sparse[val];
			dense.pop_back();
		}
	}

	void swap(SparseSet& other) noexcept {
		dense.swap(other.dense);
		sparse.swape(other.sparse);
	}


private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	container_type dense;
	container_type sparse;
};