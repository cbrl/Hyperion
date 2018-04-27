#pragma once

#include "util\datatypes\datatypes.h"
#include "ecs\handle\handle.h"


class IComponent {
	public:
		IComponent() = default;
		~IComponent() = default;

		bool operator==(const IComponent& compare) const {
			return id_hash == compare.id_hash;
		}

		bool operator!=(const IComponent& compare) const {
			return !(*this == compare);
		}

		const u64      GetID()    const { return id; }
		const Handle64 GetOwner() const { return owner; }


	private:
		u64      id_hash;
		u64      id;
		Handle64 owner;
};



template<typename T>
class Component : public IComponent {
	public:
		Component() = default;
		~Component() = default;

	public:
		// An id unique to type T
		static const type_index type_idx;
};


template<typename T>
const type_index Component<T>::type_idx = type_index(typeid(T));