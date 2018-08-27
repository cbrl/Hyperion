#pragma once

#include "memory/handle/handle.h"


//----------------------------------------------------------------------------------
// Entity Pointer
//----------------------------------------------------------------------------------
//
// Holds handle and a pointer to the entity manager. Access operators retrieve
// the entity from the entity manager.
//
//----------------------------------------------------------------------------------

class IEntity;
class EntityMgr;

class EntityPtr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EntityPtr() noexcept = default;

	EntityPtr(EntityMgr* mgr, handle64 entity) noexcept;

	EntityPtr(const EntityPtr& ptr) noexcept = default;
	EntityPtr(EntityPtr&& ptr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EntityPtr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EntityPtr& operator=(const EntityPtr& ptr) noexcept = default;
	EntityPtr& operator=(EntityPtr&& ptr) noexcept = default;

	[[nodiscard]]
	bool operator==(const EntityPtr& ptr) const {
		return handle == ptr.getHandle();
	}

	[[nodiscard]]
	bool operator!=(const EntityPtr& ptr) const {
		return !(*this == ptr);
	}

	[[nodiscard]]
	IEntity* operator->() const;

	[[nodiscard]]
	IEntity& operator*() const;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	IEntity* get() const;

	[[nodiscard]]
	handle64 getHandle() const;

	[[nodiscard]]
	bool valid() const;


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	EntityMgr* mgr;
	handle64 handle;
};