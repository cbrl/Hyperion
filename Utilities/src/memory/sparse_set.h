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
	using iterator               = typename container_type::iterator;
	using const_iterator         = typename container_type::const_iterator;
	using reverse_iterator       = typename container_type::reverse_iterator;
	using const_reverse_iterator = typename container_type::const_reverse_iterator;


	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SparseSet() noexcept = default;
	SparseSet(const SparseSet&) noexcept = default;
	SparseSet(SparseSet&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~SparseSet() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SparseSet& operator=(const SparseSet&) noexcept = default;
	SparseSet& operator=(SparseSet&&) noexcept = default;

	[[nodiscard]]
	reference operator[](size_type pos) noexcept {
		return dense[pos];
	}

	[[nodiscard]]
	const_reference operator[](size_type pos) const noexcept {
		return dense[pos];
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Access
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(const T& val) const noexcept {
		return val < _capacity     &&
		       sparse[val] < _size &&
		       dense[sparse[val]]  == val;
	}

	[[nodiscard]]
	size_type indexOf(const T& val) const noexcept {
		assert(contains(val));
		return sparse[val];
	}

	[[nodiscard]]
	reference front() noexcept {
		return dense.front();
	}

	[[nodiscard]]
	const_reference front() const noexcept {
		return dense.front();
	}

	[[nodiscard]]
	reference back() noexcept {
		return dense[_size - 1];
	}

	[[nodiscard]]
	const_reference back() const noexcept {
		return dense[_size - 1];
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
		return dense.begin();
	}

	[[nodiscard]]
	const_iterator begin() const noexcept {
		return dense.begin();
	}

	[[nodiscard]]
	const_iterator cbegin() const noexcept {
		return dense.cbegin();
	}

	[[nodiscard]]
	iterator end() noexcept {
		return dense.begin() + _size;
	}

	[[nodiscard]]
	const_iterator end() const noexcept {
		return dense.begin() + _size;
	}

	[[nodiscard]]
	const_iterator cend() const noexcept {
		return dense.cbegin() + _size;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Reverse Iterators
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	iterator rbegin() noexcept {
		return dense.rbegin();
	}

	[[nodiscard]]
	const_iterator rbegin() const noexcept {
		return dense.rbegin();
	}

	[[nodiscard]]
	const_iterator crbegin() const noexcept {
		return dense.crbegin();
	}

	[[nodiscard]]
	iterator rend() noexcept {
		return dense.rbegin() - _size;
	}

	[[nodiscard]]
	const_iterator rend() const noexcept {
		return dense.rbegin() - _size;
	}

	[[nodiscard]]
	const_iterator crend() const noexcept {
		return dense.crbegin() - _size;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Capacity
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool empty() const noexcept {
		return _size == 0;
	}

	[[nodiscard]]
	size_type size() const noexcept {
		return _size;
	}

	[[nodiscard]]
	size_type capacity() const noexcept {
		return _capacity;
	}

	void reserve(size_type new_cap) {
		if (new_cap > _capacity) {
			dense.resize(new_cap, 0);
			sparse.resize(new_cap, 0);
			_capacity = new_cap;
		}
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Modifiers
	//----------------------------------------------------------------------------------
	void clear() noexcept {
		size = 0;
	}

	// Insert the given value into the sparse set. If the set's capacity is less than
	// the value, then it will be resized to the value + 1;
	void insert(const T& val) {
		if (!contains(val)) {
			if (val >= _capacity)
				reserve(val + 1);

			dense[_size] = val;
			sparse[val]  = _size;
			++_size;
		}
	}

	void erase(const T& val) noexcept {
		if (contains(val)) {
			dense[sparse[val]]       = dense[_size - 1];
			sparse[dense[_size - 1]] = sparse[val];
			--_size;
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

	size_type _size = 0;
	size_type _capacity = 0;
};