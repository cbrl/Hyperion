#pragma once

#include "memory/sparse_set.h"

//----------------------------------------------------------------------------------
// ResourcePool
//----------------------------------------------------------------------------------
//
// Associates a handle (unsigned integer) with a resource. Resources are stored in
// contiguous blocks of memory. Creating a new resource does not invalidate iterators.
// The current resource, and only the current, can be safely deleted while iterating. 
// Pointers and references are invalidated upon modifying the container.
//
//----------------------------------------------------------------------------------


template<typename HandleT>
class IResourcePool {
public:
	//----------------------------------------------------------------------------------
	// Destructor 
	//----------------------------------------------------------------------------------
	virtual ~IResourcePool() = default;

	//----------------------------------------------------------------------------------
	// Member Functions - Modifiers
	//----------------------------------------------------------------------------------
	virtual void erase(HandleT resource_idx) = 0;
	virtual void clear() noexcept = 0;

	//----------------------------------------------------------------------------------
	// Member Functions - Access
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	virtual bool contains(HandleT val) const noexcept = 0;

	//----------------------------------------------------------------------------------
	// Member Functions - Capacity
	//----------------------------------------------------------------------------------
	virtual void reserve(size_t new_cap) = 0;
	virtual void shrink_to_fit() = 0;
};


template<typename HandleT, typename ResourceT>
class ResourcePool final: public IResourcePool<HandleT> {
	using sparse_set_type        = SparseSet<HandleT>;
	using container_type         = std::vector<ResourceT>;

public:

	using handle_type            = HandleT;
	using value_type             = ResourceT;
	using pointer                = ResourceT*;
	using const_pointer          = const ResourceT*;
	using reference              = ResourceT&;
	using const_reference        = const ResourceT&;
	using size_type              = size_t;
	using difference_type        = ptrdiff_t;


	//----------------------------------------------------------------------------------
	// Iterator
	//----------------------------------------------------------------------------------
	template<bool ConstIter>
	class iterator_t {
		friend class ResourcePool<HandleT, ResourceT>;

		using container_type = std::conditional_t<
			ConstIter,
			const typename ResourcePool<HandleT, ResourceT>::container_type,
			typename ResourcePool<HandleT, ResourceT>::container_type
		>;

		//----------------------------------------------------------------------------------
		// Constructors
		//----------------------------------------------------------------------------------
		iterator_t(container_type& resources, difference_type idx) noexcept
			: resources(&resources)
			, index(idx) {
		}

	public:

		using difference_type   = ptrdiff_t;
		using size_type         = size_t;
		using value_type        = std::conditional_t<ConstIter, const typename container_type::value_type, typename container_type::value_type>;
		using pointer           = std::conditional_t<ConstIter, typename container_type::const_pointer, typename container_type::pointer>;
		using reference         = std::conditional_t<ConstIter, typename container_type::const_reference, typename container_type::reference>;
		using iterator_category = std::random_access_iterator_tag;

		//----------------------------------------------------------------------------------
		// Constructors
		//----------------------------------------------------------------------------------
		iterator_t() noexcept = default;
		iterator_t(iterator_t&) noexcept = default;
		iterator_t(iterator_t&&) noexcept = default;

		//----------------------------------------------------------------------------------
		// Destructor
		//----------------------------------------------------------------------------------
		~iterator_t() = default;

		//----------------------------------------------------------------------------------
		// Operators - Assignment
		//----------------------------------------------------------------------------------
		iterator_t& operator=(iterator_t&) noexcept = default;
		iterator_t& operator=(iterator_t&&) noexcept = default;

		//----------------------------------------------------------------------------------
		// Operators - Access
		//----------------------------------------------------------------------------------
		[[nodiscard]]
		reference operator*() const {
			return *operator->();
		}

		[[nodiscard]]
		pointer operator->() const {
			const auto pos = static_cast<size_type>(index - 1);
			return &(*resources)[pos];
		}

		[[nodiscard]]
		reference operator[](difference_type value) const {
			const auto pos = static_cast<size_type>(index - value - 1);
			return (*resources)[pos];
		}

		//----------------------------------------------------------------------------------
		// Operators - Arithmetic
		//----------------------------------------------------------------------------------
		[[nodiscard]]
		iterator_t operator+(difference_type value) const {
			return iterator_t{*resources, index - value};
		}

		iterator_t& operator++() {
			--index;
			return *this;
		}

		iterator_t operator++(int) {
			iterator_t old = *this;
			++(*this);
			return old;
		}

		[[nodiscard]]
		iterator_t& operator+=(difference_type value) {
			index -= value;
			return *this;
		}

		[[nodiscard]]
		iterator_t operator-(difference_type value) const {
			return (*this + -value);
		}

		[[nodiscard]]
		difference_type operator-(const iterator_t& other) const {
			return other.index - index;
		}

		iterator_t& operator--() {
			++index;
			return *this;
		}

		iterator_t operator--(int) {
			iterator_t old = *this;
			--(*this);
			return old;
		}

		[[nodiscard]]
		iterator_t& operator-=(difference_type value) {
			return (*this += -value);
		}

		//----------------------------------------------------------------------------------
		// Operators - Equality
		//----------------------------------------------------------------------------------
		[[nodiscard]]
		bool operator==(const iterator_t& other) const {
			return other.index == index;
		}

		[[nodiscard]]
		bool operator!=(const iterator_t& other) const {
			return !(*this == other);
		}

		[[nodiscard]]
		bool operator<(const iterator_t& other) const {
			return index > other.index;
		}

		[[nodiscard]]
		bool operator>(const iterator_t& other) const {
			return index < other.index;
		}

		[[nodiscard]]
		bool operator<=(const iterator_t& other) const {
			return !(*this > other);
		}

		[[nodiscard]]
		bool operator>=(const iterator_t& other) const {
			return !(*this < other);
		}

	private:

		container_type* resources;
		difference_type index;
	};

	//----------------------------------------------------------------------------------
	// Iterator aliases
	//----------------------------------------------------------------------------------
	using iterator       = iterator_t<false>;
	using const_iterator = iterator_t<true>;


	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ResourcePool() = default;
	ResourcePool(const ResourcePool&) = default;
	ResourcePool(ResourcePool&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~ResourcePool() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ResourcePool& operator=(const ResourcePool&) = default;
	ResourcePool& operator=(ResourcePool&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Modifiers
	//----------------------------------------------------------------------------------
	template<typename... ArgsT>
	[[nodiscard]]
	reference construct(handle_type resource_idx, ArgsT&& ... args) {
		assert(!contains(resource_idx));
		resources.emplace_back(std::forward<ArgsT>(args)...);
		sparse_set.insert(resource_idx);
		return resources.back();
	}

	virtual void erase(handle_type resource_idx) override {
		assert(contains(resource_idx));
		auto&& back = std::move(resources.back());
		resources[sparse_set.index_of(resource_idx)] = std::move(back);
		resources.pop_back();
		sparse_set.erase(resource_idx);
	}

	virtual void clear() noexcept override {
		sparse_set.clear();
		resources.clear();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Access
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(handle_type resource_idx) const noexcept {
		return sparse_set.contains(resource_idx);
	}

	[[nodiscard]]
	reference get(handle_type resource_idx) {
		assert(contains(resource_idx));
		const auto idx = sparse_set.index_of(resource_idx);
		return resources[idx];
	}

	[[nodiscard]]
	const_reference get(handle_type resource_idx) const {
		assert(contains(resource_idx));
		const auto idx = sparse_set.index_of(resource_idx);
		return resources[idx];
	}

	[[nodiscard]]
	pointer data() noexcept {
		return resources.data();
	}

	[[nodiscard]]
	const_pointer data() const noexcept {
		return resources.data();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iterators
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	iterator begin() noexcept {
		const auto end = static_cast<typename iterator::difference_type>(resources.size());
		return iterator{resources, end};
	}

	[[nodiscard]]
	const_iterator begin() const noexcept {
		const auto end = static_cast<typename iterator::difference_type>(resources.size());
		return const_iterator{resources, end};
	}

	[[nodiscard]]
	const_iterator cbegin() const noexcept {
		const auto end = static_cast<typename iterator::difference_type>(resources.size());
		return const_iterator{resources, end};
	}

	[[nodiscard]]
	iterator end() noexcept {
		return iterator{resources, 0};
	}

	[[nodiscard]]
	const_iterator end() const noexcept {
		return const_iterator{resources, 0};
	}

	[[nodiscard]]
	const_iterator cend() const noexcept {
		return const_iterator{resources, 0};
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Capacity
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool empty() const noexcept {
		return resources.empty();
	}

	[[nodiscard]]
	size_type size() const noexcept {
		return resources.size();
	}

	virtual void reserve(size_type new_cap) override {
		sparse_set.reserve(new_cap);
		resources.reserve(new_cap);
	}

	virtual void shrink_to_fit() override {
		sparse_set.shrink_to_fit();
		resources.shrink_to_fit();
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	sparse_set_type sparse_set;
	container_type  resources;
};