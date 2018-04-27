#pragma once

#include "util\datatypes\datatypes.h"
#include "ecs\handle\handle.h"


//----------------------------------------------------------------------------------
// IComponent
//----------------------------------------------------------------------------------
//
// Base class for the Component class. Only to be used by that class.
//
//----------------------------------------------------------------------------------

class IComponent {
	friend class IEntity;

	public:
		IComponent()
			: owner(Handle64::invalid_handle)
			, active(true)
		{}

		virtual ~IComponent() = default;

		const Handle64 GetOwner() const {
			return owner;
		}

		void SetActive(bool state) { active = state; }
		bool IsActive() const { return active; }

		virtual const type_index GetTypeIDX() const = 0;


	protected:
		bool     active;

		// Set on creation in IEntity
		Handle64 owner;
};



//----------------------------------------------------------------------------------
// Component
//----------------------------------------------------------------------------------
//
// Base class that all unique components will derive from
//
//----------------------------------------------------------------------------------

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