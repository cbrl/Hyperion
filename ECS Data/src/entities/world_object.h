#pragma once

#include "entity/entity.h"
#include "components/transform/transform.h"


//----------------------------------------------------------------------------------
// WorldObjecct
//----------------------------------------------------------------------------------
//
// The base class that any object with a position in the world
// will inherit from. Automatically adds a Transform component.
//
//----------------------------------------------------------------------------------

template<typename T = void>
class WorldObject : public Entity<WorldObject<T>> {
public:
	WorldObject(handle64 this_handle, ComponentMgr* component_mgr)
		: Entity<WorldObject<T>>(this_handle, component_mgr) {

		this->template addComponent<Transform>();
	}

	virtual ~WorldObject() = default;

	//	template<typename... ArgsT>
	//	void Init(ArgsT&&... args) {
	//		this->AddComponent<Transform>();
	//		InitImpl(std::is_same<T, void>{}, std::forward<ArgsT>(args)...);
	//	}


	//protected:
	//	template<typename... ArgsT>
	//	void InitImpl(std::false_type, ArgsT&&... args) {
	//		if (&WorldObject::Init != &T::Init)
	//			static_cast<T*>(this)->Init(std::forward<ArgsT>(args)...);
	//	}
};