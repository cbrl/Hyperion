#pragma once

#include "component\component_mgr.h"


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

		template<typename ComponentT, typename... ArgsT>
		ComponentT* const AddComponent(ArgsT&&... args);

		template<typename ComponentT>
		[[nodiscard]]
		ComponentT* const GetComponent() const;

		template<typename ComponentT>
		void RemoveComponent();


	private:
		void SetHandle(Handle64 this_handle);
		void SetComponentMgr(ComponentMgr* mgr);


	protected:
		// Is this entity active or should it be ignored?
		bool active;

		// Handle to this entity. Set on creation in EntityMgr.
		Handle64 handle;

		// A pointer to the component manager. The ECS destroys all
		// entities before the component manager is destroyed, so
		// this pointer should never be invalid in this context.
		ComponentMgr* component_mgr;

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
	friend class EntityMgr;

	public:
		virtual const type_index GetTypeID() const override {
			return type_id;
		}


	protected:
		Entity() = default;
		Entity(const Entity& entity) = default;
		Entity(Entity&& entity) = delete;
		virtual ~Entity() = default;

		template<typename... ArgsT>
		void Construct(ArgsT&&... args);

		template<typename... ArgsT>
		void Init(ArgsT&&... args);


	public:
		// And ID unique to type T
		static const type_index type_id;
};


template<typename T>
const type_index Entity<T>::type_id = type_index(typeid(T));


#include "entity.tpp"