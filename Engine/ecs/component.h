#pragma once

#include "util\datatypes\datatypes.h"
#include "ecs\handle\handle.h"


class IComponent {
	friend class IEntity;

	public:
		IComponent() = default;
		virtual ~IComponent() = default;

		const Handle64 GetOwner() const {
			return owner;
		}

		virtual const type_index GetTypeIDX() const = 0;


	protected:
		Handle64 owner;
};



template<typename T>
class Component : public IComponent {
	public:
		Component() = default;
		virtual ~Component() = default;

		const type_index GetTypeIDX() const {
			return type_idx;
		}


	public:
		// An ID unique to type T
		static const type_index type_idx;
};


template<typename T>
const type_index Component<T>::type_idx = type_index(typeid(T));