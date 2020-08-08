#pragma once

#include "datatypes/pointer_types.h"
#include "datatypes/container_types.h"
#include <concepts>


//----------------------------------------------------------------------------------
// SharedResourceMap
//----------------------------------------------------------------------------------
//
// Holds shared_ptrs to resources of the provided template type.
//
//----------------------------------------------------------------------------------
template<typename KeyT, typename ResourceT, template<typename, typename, typename...> typename MapT = std::unordered_map>
class SharedResourceMap final : public MapT<KeyT, std::shared_ptr<ResourceT>> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SharedResourceMap() = default;
	SharedResourceMap(const SharedResourceMap&) = default;
	SharedResourceMap(SharedResourceMap&&) = default;
	

	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~SharedResourceMap() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SharedResourceMap& operator=(const SharedResourceMap&) = default;
	SharedResourceMap& operator=(SharedResourceMap&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Create a resource, or retrieve it if it already exists.
	template<typename... ArgsT>
	[[nodiscard]]
	std::shared_ptr<ResourceT> getOrCreate(const KeyT& key, ArgsT&&... args) {
		return getOrCreateDerived<ResourceT>(key, std::forward<ArgsT>(args)...);
	}

	// Create a resource of a derived type, or retrieve it if it already exists.
	template<typename DerivedT, typename... ArgsT> requires std::derived_from<DerivedT, ResourceT>
	[[nodiscard]]
	std::shared_ptr<ResourceT> getOrCreateDerived(const KeyT& key, ArgsT&&... args) {
		if (const auto it = this->find(key); it != this->end()) {
			return it->second;
		}
		const auto resource = std::make_shared<DerivedT>(std::forward<ArgsT>(args)...);
		this->insert(std::make_pair(key, resource));
		return resource;
	}

	// Create a resource, or replace it if it already exists.
	template<typename... ArgsT>
	[[nodiscard]]
	std::shared_ptr<ResourceT> createOrReplace(const KeyT& key, ArgsT&&... args) {
		return createOrReplaceDerived<ResourceT>(key, std::forward<ArgsT>(args)...);
	}

	// Create a resource of a derived type, or replace it if it already exists.
	template<typename DerivedT, typename... ArgsT> requires std::derived_from<DerivedT, ResourceT>
	[[nodiscard]]
	std::shared_ptr<ResourceT> createOrReplaceDerived(const KeyT& key, ArgsT&&... args) {
		const auto resource = std::make_shared<DerivedT>(std::forward<ArgsT>(args)...);
		this->operator[](key) = resource;
		return resource;
	}
};



//----------------------------------------------------------------------------------
// WeakResourceMap
//----------------------------------------------------------------------------------
//
// Holds weak_ptrs to resources of the provided template type, and returns
// shared_ptrs to those resources. Resources will automatically be destroyed when
// all shared_ptrs are destroyed.
//
//----------------------------------------------------------------------------------
template<typename KeyT, typename ResourceT, template <typename, typename, typename...> typename MapT = std::unordered_map>
class WeakResourceMap final {
private:
	using key_type      = KeyT;
	using resource_type = ResourceT;
	using shared_ptr    = std::shared_ptr<resource_type>;
	using map_type      = MapT<key_type, std::weak_ptr<resource_type>>;

private:

	//----------------------------------------------------------------------------------
	// Iterator
	//----------------------------------------------------------------------------------
	template<bool Const>
	class iterator_t {
		friend class WeakResourceMap<key_type, resource_type, MapT>;
		
		using map_iterator = std::conditional_t<Const, typename map_type::const_iterator, typename map_type::iterator>;

		// Private map constructor
		iterator_t(map_iterator it) : it(it) {}

	public:

		using kv_pair         = std::pair<const key_type&, shared_ptr>;
		using difference_type = ptrdiff_t;

		//----------------------------------------------------------------------------------
		// Constructors
		//----------------------------------------------------------------------------------
		iterator_t() noexcept = default;
		iterator_t(const iterator_t&) noexcept = default;
		iterator_t(iterator_t&&) noexcept = default;

		//----------------------------------------------------------------------------------
		// Destructor
		//----------------------------------------------------------------------------------
		~iterator_t() = default;

		//----------------------------------------------------------------------------------
		// Operators - Assignment
		//----------------------------------------------------------------------------------
		iterator_t& operator=(const iterator_t&) noexcept = default;
		iterator_t& operator=(iterator_t&&) noexcept = default;

		//----------------------------------------------------------------------------------
		// Operators - Access
		//----------------------------------------------------------------------------------
		[[nodiscard]]
		kv_pair operator*() {
			return kv_pair{ it->first, it->second.lock() };
		}

		[[nodiscard]]
		kv_pair operator->() {
			return kv_pair{ it->first, it->second.lock() };
		}

		//----------------------------------------------------------------------------------
		// Operators - Arithmetic
		//----------------------------------------------------------------------------------
		iterator_t& operator++() {
			++it;
			return *this;
		}

		iterator_t operator++(int) {
			iterator_t old = *this;
			++(*this);
			return old;
		}

		[[nodiscard]]
		iterator_t operator+(difference_type value) {
			return iterator_t{it + value};
		}

		[[nodiscard]]
		iterator_t& operator+=(difference_type value) {
			it += value;
			return *this;
		}

		[[nodiscard]]
		iterator_t operator-(difference_type value) const {
			return (*this + -value);
		}

		[[nodiscard]]
		difference_type operator-(const iterator_t& rhs) const {
			return it - rhs.it;
		}

		iterator_t& operator--() {
			--it;
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
		bool operator<(const iterator_t& rhs) const noexcept {
			return it < rhs.it;
		}

		[[nodiscard]]
		bool operator>(const iterator_t& rhs) const noexcept {
			return !(*this < rhs);
		}

		[[nodiscard]]
		bool operator<=(const iterator_t& rhs) const noexcept {
			return !(*this > rhs);
		}

		[[nodiscard]]
		bool operator>=(const iterator_t& rhs) const noexcept {
			return !(*this < rhs);
		}

		[[nodiscard]]
		bool operator==(const iterator_t& rhs) const noexcept {
			return it == rhs.it;
		}

		[[nodiscard]]
		bool operator!=(const iterator_t& rhs) const noexcept {
			return !(*this == rhs);
		}

	private:

		//----------------------------------------------------------------------------------
		// Member Variables
		//----------------------------------------------------------------------------------
		map_iterator it;
	};

	using iterator = iterator_t<false>;
	using const_iterator = iterator_t<true>;

public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	WeakResourceMap() = default;
	WeakResourceMap(const WeakResourceMap& map) = default;
	WeakResourceMap(WeakResourceMap&& map) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~WeakResourceMap() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	WeakResourceMap& operator=(const WeakResourceMap& map) = default;
	WeakResourceMap& operator=(WeakResourceMap&& map) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Creation/Access
	//----------------------------------------------------------------------------------

	// Create a resource, or retrieve it if it already exists.
	template<typename... ArgsT>
	[[nodiscard]]
	shared_ptr getOrCreate(const key_type& key, ArgsT&& ... args) {
		return getOrCreateDerived<resource_type>(key, std::forward<ArgsT>(args)...);
	}

	// Create a resource of a derived type, or retrieve it if it already exists.
	template<typename DerivedT, typename... ArgsT> requires std::derived_from<DerivedT, ResourceT>
	[[nodiscard]]
	shared_ptr getOrCreateDerived(const key_type& key, ArgsT&& ... args) {
		if (const auto it = resource_map.find(key); it != resource_map.end() && !it->second.expired()) {
			return it->second.lock(); //Return the resource if it exists and hasn't expired
		}

		// Create the resource if it doesn't exist or was expired
		const auto resource = std::make_shared<DerivedT>(std::forward<ArgsT>(args)...);
		resource_map[key] = resource;
		return resource;
	}

	// Create a resource, or replace it if it already exists.
	template<typename... ArgsT>
	[[nodiscard]]
	shared_ptr createOrReplace(const key_type& key, ArgsT&& ... args) {
		return createOrReplaceDerived<resource_type>(key, std::forward<ArgsT>(args)...);
	}

	// Create a resource of a derived type, or replace it if it already exists.
	template<typename DerivedT, typename... ArgsT> requires std::derived_from<DerivedT, ResourceT>
	[[nodiscard]]
	shared_ptr createOrReplaceDerived(const key_type& key, ArgsT&&... args) {
		const auto resource = std::make_shared<DerivedT>(std::forward<ArgsT>(args)...);
		resource_map[key] = resource;
		return resource;		
	}

	[[nodiscard]]
	shared_ptr at(const key_type& key) { return resource_map.at(key).lock(); }

	[[nodiscard]]
	iterator find(const key_type& key) { return iterator{resource_map.find(key)}; }


	//----------------------------------------------------------------------------------
	// Member Functions - Iterators
	//----------------------------------------------------------------------------------
	[[nodiscard]] iterator       begin()        { return iterator{resource_map.begin()}; }
	[[nodiscard]] const_iterator begin()  const { return const_iterator{resource_map.begin()}; }
	[[nodiscard]] const_iterator cbegin() const { return const_iterator{resource_map.cbegin()}; }
	[[nodiscard]] iterator       end()          { return iterator{resource_map.end()}; }
	[[nodiscard]] const_iterator end()    const { return const_iterator{resource_map.end()}; }
	[[nodiscard]] const_iterator cend()   const { return const_iterator{resource_map.cend()}; }


	//----------------------------------------------------------------------------------
	// Member Functions - Capacity
	//----------------------------------------------------------------------------------
	[[nodiscard]] bool   empty()    const noexcept { return resource_map.empty(); }
	[[nodiscard]] size_t size()     const noexcept { return resource_map.size(); }
	[[nodiscard]] size_t max_size() const noexcept { return resource_map.max_size(); }

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	map_type resource_map;
};



//----------------------------------------------------------------------------------
// Concurrent Maps (Thread safe iteration and insertion)
//----------------------------------------------------------------------------------
#include <concurrent_unordered_map.h>

template<typename KeyT, typename ResourceT>
using ConcurrentSharedResourceMap = SharedResourceMap<KeyT, ResourceT, concurrency::concurrent_unordered_map>;

template<typename KeyT, typename ResourceT>
using ConcurrentWeakResourceMap = WeakResourceMap<KeyT, ResourceT, concurrency::concurrent_unordered_map>;
