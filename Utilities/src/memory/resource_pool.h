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
	using container_type = std::pmr::list<T>;

public:
	ResourcePool() : container_type(&pool_resource) {}

	//----------------------------------------------------------------------------------
	// Modifiers
	//----------------------------------------------------------------------------------
	using container_type::emplace_front;
	using container_type::emplace_back;

	//----------------------------------------------------------------------------------
	// Operations
	//----------------------------------------------------------------------------------

	// Remove a resource referenced to by its pointer
	void remove_resource(void* resource) {
		const auto num_removed = this->remove_if([resource](T& element) {
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
	using container_type::begin;
	using container_type::cbegin;
	using container_type::rbegin;
	using container_type::crbegin;
	using container_type::end;
	using container_type::cend;
	using container_type::rend;
	using container_type::crend;

	//----------------------------------------------------------------------------------
	// Capacity
	//----------------------------------------------------------------------------------
	using container_type::empty;
	using container_type::size;
	using container_type::max_size;
};
