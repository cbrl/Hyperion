#pragma once

#include <vector>
#include <type_traits>
#include <assert.h>


template<typename T>
class SparseSet final {
	static_assert(std::is_integral_v<T> && std::is_unsigned_v<T>,
	              "Sparse sets only support unsigned integral types");

	using container_type         = std::vector<T>;

public:

	using value_type             = T;
	using pointer                = T*;
	using const_pointer          = const T*;
	using reference              = T&;
	using const_reference        = const T&;
	using size_type              = size_t;
	using difference_type        = ptrdiff_t;

	// The state of a sparse set should only be modified through its member
	// functions, so the only iterator type is a const iterator. The underlying
	// dense container is reverse iterated. This ensures that the current element
	// can be deleted while iterating and no other elements will be skipped.
	// However, elements added while iterating will not be covered.
	class const_iterator {
		friend class SparseSet<T>;
		using container_type = SparseSet<T>::container_type;

		const_iterator(const container_type& dense, difference_type idx) noexcept
			: dense(&dense)
			, index(idx) {
		}

	public:

		using difference_type   = ptrdiff_t;
		using size_type         = size_t;
		using value_type        = SparseSet<T>::value_type;
		using const_pointer     = SparseSet<T>::const_pointer;
		using reference         = SparseSet<T>::reference;
		using const_reference   = SparseSet<T>::const_reference;
		using iterator_category = std::random_access_iterator_tag;

		const_iterator() noexcept = default;
		const_iterator(const_iterator&) noexcept = default;
		const_iterator(const_iterator&&) noexcept = default;

		~const_iterator() = default;

		const_iterator& operator=(const_iterator&) noexcept = default;
		const_iterator& operator=(const_iterator&&) noexcept = default;

		[[nodiscard]]
		const_reference operator*() const {
			return *operator->();
		}

		[[nodiscard]]
		const_pointer operator->() const {
			const auto pos = static_cast<size_type>(index - 1);
			return &(*dense)[pos];
		}

		[[nodiscard]]
		const_reference operator[](difference_type value) const {
			const auto pos = static_cast<size_type>(index - value - 1);
			return (*dense)[pos];
		}

		[[nodiscard]]
		const_iterator operator+(difference_type value) const {
			return const_iterator{*dense, index - value};
		}

		const_iterator& operator++() {
			--index;
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator old = *this;
			++(*this);
			return old;
		}

		[[nodiscard]]
		const_iterator& operator+=(difference_type value) {
			index -= value;
			return *this;
		}

		[[nodiscard]]
		const_iterator operator-(difference_type value) const {
			return (*this + -value);
		}

		[[nodiscard]]
		difference_type operator-(const const_iterator& other) const {
			return other.index - index;
		}

		const_iterator& operator--() {
			++index;
			return *this;
		}

		const_iterator operator--(int) {
			const_iterator old = *this;
			--(*this);
			return old;
		}

		[[nodiscard]]
		const_iterator& operator-=(difference_type value) {
			return (*this += -value);
		}

		[[nodiscard]]
		bool operator==(const const_iterator& other) const {
			return other.index == index;
		}

		[[nodiscard]]
		bool operator!=(const const_iterator& other) const {
			return !(*this == other);
		}

		[[nodiscard]]
		bool operator<(const const_iterator& other) const {
			return index > other.index;
		}

		[[nodiscard]]
		bool operator>(const const_iterator& other) const {
			return index < other.index;
		}

		[[nodiscard]]
		bool operator<=(const const_iterator& other) const {
			return !(*this > other);
		}

		[[nodiscard]]
		bool operator>=(const const_iterator& other) const {
			return !(*this < other);
		}

	private:

		const container_type* dense;
		difference_type index;
	};


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
	const_iterator begin() const noexcept {
		const auto end = static_cast<typename const_iterator::difference_type>(dense.size());
		return const_iterator{dense, end};
	}

	[[nodiscard]]
	const_iterator cbegin() const noexcept {
		const auto end = static_cast<typename const_iterator::difference_type>(dense.size());
		return const_iterator{dense, end};
	}

	[[nodiscard]]
	const_iterator end() const noexcept {
		return const_iterator{dense, 0};
	}

	[[nodiscard]]
	const_iterator cend() const noexcept {
		return const_iterator{dense, 0};
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

	void reserve(size_type new_cap) {
		if (new_cap > sparse.size()) {
			dense.reserve(new_cap);
			sparse.resize(new_cap, 0);
		}
	}

	void shrink_to_fit() {
		if (dense.empty()) {
			sparse.clear();
		}

		dense.shrink_to_fit();
		sparse.shrink_to_fit();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Modifiers
	//----------------------------------------------------------------------------------
	void clear() noexcept {
		dense.clear();
		sparse.clear();
	}

	// Insert the given value into the sparse set. If the set's capacity is less than
	// the value, then it will be resized to the value + 1;
	void insert(value_type val) {
		if (!contains(val)) {
			if (val >= sparse.size())
				reserve(val + 1);

			sparse[val] = dense.size();
			dense.push_back(val);
		}
	}

	void erase(value_type val) {
		if (contains(val)) {
			dense[sparse[val]] = dense.back();
			sparse[dense.back()] = sparse[val];
			dense.pop_back();
		}
	}

	void swap(SparseSet& other) noexcept {
		dense.swap(other.dense);
		sparse.swap(other.sparse);
	}


private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	container_type dense;
	container_type sparse;
};