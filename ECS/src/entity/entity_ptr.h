#pragma once

#include "memory/handle/handle.h"


//----------------------------------------------------------------------------------
// EntityPtr
//----------------------------------------------------------------------------------
//
// Holds an entity handle and a pointer to the entity manager. Access operators
// retrieve the entity from the entity manager.
//
//----------------------------------------------------------------------------------

class Entity;
class EntityMgr;

class EntityPtr {
	friend class UniqueEntityPtr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EntityPtr() noexcept
		: mgr(nullptr)
		, handle(handle64::invalid_handle) {
	}

	EntityPtr(EntityMgr* mgr, handle64 entity) noexcept
		: mgr(mgr)
		, handle(entity) {
	}

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
	bool operator==(const EntityPtr& ptr) const noexcept {
		return handle == ptr.getHandle();
	}

	[[nodiscard]]
	bool operator!=(const EntityPtr& ptr) const noexcept {
		return !(*this == ptr);
	}

	[[nodiscard]]
	Entity* operator->() const;

	[[nodiscard]]
	Entity& operator*() const;

	[[nodiscard]]
	operator bool() const {
		return valid();
	}


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	Entity* get() const;

	[[nodiscard]]
	handle64 getHandle() const noexcept;

	[[nodiscard]]
	bool valid() const noexcept;


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	EntityMgr* mgr;
	handle64 handle;
};




/*
//----------------------------------------------------------------------------------
// UniqueEntityPtr
//----------------------------------------------------------------------------------
//
// An std::unique_ptr analogue for EntityPtr.
//
//----------------------------------------------------------------------------------

class UniqueEntityPtr final : public EntityPtr {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	UniqueEntityPtr() noexcept = default;

	UniqueEntityPtr(EntityMgr* mgr, handle64 entity) noexcept
		: EntityPtr(mgr, entity) {
	}

	UniqueEntityPtr(const EntityPtr& entity) noexcept
		: EntityPtr(entity) {
	}

	UniqueEntityPtr(const UniqueEntityPtr& ptr) = delete;

	UniqueEntityPtr(UniqueEntityPtr&& ptr) noexcept
		: EntityPtr(ptr.release()) {
	}


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~UniqueEntityPtr() {
		reset();
	}


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	UniqueEntityPtr& operator=(const UniqueEntityPtr& entity_ptr) = delete;
	
	UniqueEntityPtr& operator=(UniqueEntityPtr&& ptr) noexcept;

	[[nodiscard]]
	bool operator==(const UniqueEntityPtr& ptr) const noexcept {
		return EntityPtr::operator==(static_cast<EntityPtr&>(ptr));
	}

	[[nodiscard]]
	bool operator!=(const UniqueEntityPtr& ptr) const noexcept {
		return EntityPtr::operator!=(static_cast<EntityPtr&>(ptr));
	}

	[[nodiscard]]
	bool operator==(const EntityPtr& ptr) const noexcept {
		return EntityPtr::operator==(ptr);
	}

	[[nodiscard]]
	bool operator!=(const EntityPtr& ptr) const noexcept {
		return EntityPtr::operator!=(ptr);
	}


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void reset();

private:
	[[nodiscard]]
	EntityPtr release() noexcept;
};
*/