#pragma once

#include "memory/handle/handle.h"


//----------------------------------------------------------------------------------
// EntityPtr
//----------------------------------------------------------------------------------
//
// Holds handle and a pointer to the entity manager. Access operators retrieve
// the entity from the entity manager.
//
//----------------------------------------------------------------------------------

class IEntity;
class EntityMgr;

class EntityPtr final {
	//friend class UniqueEntityPtr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EntityPtr() noexcept;

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
	bool operator==(const EntityPtr& ptr) const noexcept {
		return handle == ptr.getHandle();
	}

	[[nodiscard]]
	bool operator!=(const EntityPtr& ptr) const noexcept {
		return !(*this == ptr);
	}

	[[nodiscard]]
	IEntity* operator->() const;

	[[nodiscard]]
	IEntity& operator*() const;

	[[nodiscard]]
	operator bool() const;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	IEntity* get() const;

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

class UniqueEntityPtr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	UniqueEntityPtr() noexcept = default;

	UniqueEntityPtr(EntityMgr* mgr, handle64 entity) noexcept;
	UniqueEntityPtr(const EntityPtr& entity) noexcept;

	UniqueEntityPtr(const UniqueEntityPtr& ptr) = delete;
	UniqueEntityPtr(UniqueEntityPtr&& ptr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~UniqueEntityPtr();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	UniqueEntityPtr& operator=(const UniqueEntityPtr& entity_ptr) = delete;
	UniqueEntityPtr& operator=(UniqueEntityPtr&& entity_ptr) noexcept;

	[[nodiscard]]
	bool operator==(const UniqueEntityPtr& entity_ptr) const noexcept {
		return this->ptr == entity_ptr.ptr;
	}

	[[nodiscard]]
	bool operator!=(const UniqueEntityPtr& entity_ptr) const noexcept {
		return !(*this == ptr);
	}

	[[nodiscard]]
	bool operator==(const EntityPtr& ptr) const noexcept {
		return this->ptr == ptr;
	}

	[[nodiscard]]
	bool operator!=(const EntityPtr& ptr) const noexcept {
		return !(*this == ptr);
	}

	[[nodiscard]]
	IEntity* operator->() const;

	[[nodiscard]]
	IEntity& operator*() const;

	[[nodiscard]]
	operator bool() const;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	IEntity* get() const;

	[[nodiscard]]
	EntityPtr getPtr() const noexcept;

	[[nodiscard]]
	handle64 getHandle() const noexcept;

	void reset();


private:
	EntityPtr ptr;
};
*/