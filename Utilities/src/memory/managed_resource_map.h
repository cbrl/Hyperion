#pragma once

#include "datatypes/pointer_types.h"
#include "datatypes/container_types.h"


//----------------------------------------------------------------------------------
// SharedResourceMap
//----------------------------------------------------------------------------------
//
// Holds shared_ptrs to resources of the provided template type.
//
//----------------------------------------------------------------------------------
template<typename KeyT, typename ValueT, template<typename, typename, typename...> typename MapT = std::unordered_map>
class SharedResourceMap final : public MapT<KeyT, std::shared_ptr<ValueT>> {
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
	std::shared_ptr<ValueT> getOrCreate(const KeyT& key, ArgsT&&... args) {
		return getOrCreateDerived<ValueT>(key, std::forward<ArgsT>(args)...);
	}

	// Create a resource of a derived type, or retrieve it if it already exists.
	template<typename DerivedT, typename... ArgsT>
	[[nodiscard]]
	std::shared_ptr<ValueT> getOrCreateDerived(const KeyT& key, ArgsT&&... args) {
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
	std::shared_ptr<ValueT> createOrReplace(const KeyT& key, ArgsT&&... args) {
		return createOrReplaceDerived<ValueT>(key, std::forward<ArgsT>(args)...);
	}

	// Create a resource of a derived type, or replace it if it already exists.
	template<typename DerivedT, typename... ArgsT>
	[[nodiscard]] std::shared_ptr<ValueT> createOrReplaceDerived(const KeyT& key, ArgsT&&... args) {
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
template<typename KeyT, typename ValueT, template <typename, typename, typename...> typename MapT = std::unordered_map>
class WeakResourceMap final {
public:
	using key_type   = KeyT;
	using value_type = ValueT;
	using map_type   = MapT<key_type, std::weak_ptr<value_type>>;

private:
	class iterator {
		friend class WeakResourceMap<key_type, value_type, MapT>;

	public:
		using map_iterator   = typename map_type::iterator;
		using shared_pointer = std::shared_ptr<value_type>;
		using pair_kp        = std::pair<const key_type&, shared_pointer>;

		iterator(map_iterator it) : it(it) {}

		[[nodiscard]] pair_kp operator*()  { return pair_kp{it->first, it->second.lock()}; }
		[[nodiscard]] pair_kp operator->() { return pair_kp{it->first, it->second.lock()}; }

		iterator& operator++() { ++it; return *this; }

		bool operator==(const iterator& rhs) const noexcept { return it == rhs.it; }
		bool operator!=(const iterator& rhs) const noexcept { return it != rhs.it; }

	private:
		map_iterator it;
	};


	class const_iterator {
		friend class WeakResourceMap<key_type, value_type, MapT>;

	public:
		using const_map_iterator = typename map_type::const_iterator;
		using shared_pointer     = std::shared_ptr<value_type>;
		using pair_kp            = std::pair<const key_type&, shared_pointer>;

		const_iterator(const_map_iterator it) : it(it) {}

		[[nodiscard]] pair_kp operator*()  const { return pair_kp{it->first, it->second.lock()}; }
		[[nodiscard]] pair_kp operator->() const { return pair_kp{it->first, it->second.lock()}; }

		const_iterator& operator++() { ++it; return *this; }

		bool operator==(const const_iterator& rhs) const noexcept { return it == rhs.it; }
		bool operator!=(const const_iterator& rhs) const noexcept { return it != rhs.it; }

	private:
		const_map_iterator it;
	};


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
	std::shared_ptr<value_type> getOrCreate(const key_type& key, ArgsT&& ... args) {
		return getOrCreateDerived<value_type>(key, std::forward<ArgsT>(args)...);
	}

	// Create a resource of a derived type, or retrieve it if it already exists.
	template<typename DerivedT, typename... ArgsT>
	[[nodiscard]]
	std::shared_ptr<value_type> getOrCreateDerived(const key_type& key, ArgsT&& ... args) {

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
	std::shared_ptr<value_type> createOrReplace(const key_type& key, ArgsT&& ... args) {
		return createOrReplaceDerived<value_type>(key, std::forward<ArgsT>(args)...);
	}

	// Create a resource of a derived type, or replace it if it already exists.
	template<typename DerivedT, typename... ArgsT>
	[[nodiscard]]
	std::shared_ptr<value_type> createOrReplaceDerived(const key_type& key, ArgsT&&... args) {
		const auto resource = std::make_shared<DerivedT>(std::forward<ArgsT>(args)...);
		resource_map[key] = resource;
		return resource;		
	}

	[[nodiscard]]
	std::shared_ptr<value_type> at(const key_type& key) { return resource_map.at(key).lock(); }

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

template<typename KeyT, typename ValueT>
using ConcurrentSharedResourceMap = SharedResourceMap<KeyT, ValueT, concurrency::concurrent_unordered_map>;

template<typename KeyT, typename ValueT>
using ConcurrentWeakResourceMap = WeakResourceMap<KeyT, ValueT, concurrency::concurrent_unordered_map>;
