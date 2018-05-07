#pragma once

#include "ecs\component\component_mgr.h"


//----------------------------------------------------------------------------------
// IEntity
//----------------------------------------------------------------------------------
//
// Base class for the Entity class. Only to be used by that class.
//
//----------------------------------------------------------------------------------

class IEntity {
	friend class EntityMgr;

	public:
		IEntity();
		virtual ~IEntity();

		virtual const type_index GetTypeID() const = 0;

		void SetActive(bool state);
		bool IsActive() const;

		[[nodiscard]]
		const Handle64 GetHandle() const;

		template<typename ComponentT>
		[[nodiscard]]
		ComponentT* GetComponent() const;

		template<typename ComponentT, typename... ArgsT>
		ComponentT* AddComponent(ArgsT&&... args);

		template<typename ComponentT>
		void RemoveComponent();


	protected:
		// Is this entity active or should it be ignored?
		bool active;

		// Handle to this entity. Set on creation in EntityMgr.
		Handle64 handle;

		// Map of pointers to components. Holds 1 of each unique type.
		unordered_map<type_index, IComponent*> components;
};



//----------------------------------------------------------------------------------
// Entity
//----------------------------------------------------------------------------------
//
// Base class that all unique entities will derive from
//
//----------------------------------------------------------------------------------

template<typename T>
class Entity : public IEntity {
	public:
		Entity() = default;
		virtual ~Entity() = default;

		virtual const type_index GetTypeID() const override {
			return type_id;
		}


	public:
		// And ID unique to type T
		static const type_index type_id;
};


template<typename T>
const type_index Entity<T>::type_id = type_index(typeid(T));


#include "entity.tpp"