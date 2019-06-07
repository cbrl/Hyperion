#pragma once

#include "datatypes/datatypes.h"
#include "exception/exception.h"
#include "log/log.h"
#include <memory_resource>


//----------------------------------------------------------------------------------
// ResourcePool
//----------------------------------------------------------------------------------
//
// Constructs resources of type T using a std::list with the
// std::pmr::synchronized_pool_resource memory resource.
//
//----------------------------------------------------------------------------------

class IResourcePool {
public:
	virtual ~IResourcePool() = default;

	// Remove a resource referenced to by its pointer
	virtual void remove_resource(void* resource) = 0;

protected:
	std::pmr::synchronized_pool_resource pool_resource;
};

template <typename T>
class ResourcePool final : public IResourcePool, private std::pmr::list<T> {
public:
	ResourcePool() : std::pmr::list<T>(&pool_resource) {}

	//----------------------------------------------------------------------------------
	// Modifiers
	//----------------------------------------------------------------------------------
	using std::pmr::list<T>::emplace_front;
	using std::pmr::list<T>::emplace_back;

	//----------------------------------------------------------------------------------
	// Operations
	//----------------------------------------------------------------------------------

	// Remove a resource referenced to by its pointer
	void remove_resource(void* resource) {
		const auto num_removed = this->remove_if([resource, &found](T& element) {
			return &element == resource;
		});
		if (num_removed == 0)
			Logger::log(LogLevel::warn, "ResourcePool::remove_resource() found no object to remove");
	}

	// Remove a resource referenced to by its pointer
	void remove_resource(T& resource) {
		remove_resource(&resource);
	}

	//----------------------------------------------------------------------------------
	// Iterators
	//----------------------------------------------------------------------------------
	using std::pmr::list<T>::begin;
	using std::pmr::list<T>::cbegin;
	using std::pmr::list<T>::rbegin;
	using std::pmr::list<T>::crbegin;
	using std::pmr::list<T>::end;
	using std::pmr::list<T>::cend;
	using std::pmr::list<T>::rend;
	using std::pmr::list<T>::crend;

	//----------------------------------------------------------------------------------
	// Capacity
	//----------------------------------------------------------------------------------
	using std::pmr::list<T>::empty;
	using std::pmr::list<T>::size;
	using std::pmr::list<T>::max_size;
};
