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
		//----------------------------------------------------------------------------------
		// Constructors / Destructors
		//----------------------------------------------------------------------------------

		IEntity() = delete;

		IEntity(const IEntity& entity) = delete;

		IEntity(IEntity&& entity) = default;

		IEntity(Handle64 this_handle, ComponentMgr* component_mgr);

		virtual ~IEntity();


		//----------------------------------------------------------------------------------
		// Member functions
		//----------------------------------------------------------------------------------

		// Interface function for retrieving the type_index
		virtual const type_index GetTypeID() const = 0;

		// Get the entity's handle
		[[nodiscard]]
		const Handle64 GetHandle() const;

		// Get or set the state of this entity
		void SetActive(bool state);
		bool IsActive() const;

		// Add a component to this entity
		template<typename ComponentT, typename... ArgsT>
		ComponentT* const AddComponent(ArgsT&&... args);

		// Get a component that's been added to this entity
		template<typename ComponentT>
		[[nodiscard]]
		ComponentT* const GetComponent() const;

		// Remove a component from this entity
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
		Entity() = delete;

		Entity(Handle64 this_handle, ComponentMgr* component_mgr)
			: IEntity(this_handle, component_mgr)
		{}

		Entity(const Entity& entity) = delete;

		Entity(Entity&& entity) = default;

		virtual ~Entity() = default;


	public:
		// And ID unique to type T
		static const type_index type_id;
};


template<typename T>
const type_index Entity<T>::type_id = type_index(typeid(T));


#include "entity.tpp"
