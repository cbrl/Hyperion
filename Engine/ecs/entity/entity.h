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
		IEntity() : active(true) {}

		virtual ~IEntity() {
			if (auto tmp = component_mgr.lock()) {
				for (auto& pair : components) {
					tmp->DestroyComponent(pair.second);
					pair.second = nullptr;
				}
			}
		}

		void SetActive(bool state) { active = state; }
		bool IsActive() const { return active; }

		virtual const type_index GetTypeIDX() const = 0;

		[[nodiscard]]
		const Handle64 GetHandle() const { return handle; }

		template<typename ComponentT>
		[[nodiscard]]
		ComponentT* GetComponent() const;

		template<typename ComponentT, typename... ArgsT>
		void AddComponent(ArgsT&&... args);

		template<typename ComponentT>
		void RemoveComponent();


	protected:
		bool     active;

		// Set on creation in EntityMgr
		Handle64 handle;

		// Map of components. Holds 1 of each unique type.
		unordered_map<type_index, IComponent*> components;


	private:
		// Pointer to the ComponentMgr
		weak_ptr<ComponentMgr> component_mgr;
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

		virtual const type_index GetTypeIDX() const {
			return type_idx;
		}


	public:
		// And ID unique to type T
		static const type_index type_idx;
};


template<typename T>
const type_index Entity<T>::type_idx = type_index(typeid(T));


#include "entity.tpp"