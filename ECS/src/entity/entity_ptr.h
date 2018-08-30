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
	IEntity* operator->() const;

	[[nodiscard]]
	IEntity& operator*() const;

	[[nodiscard]]
	operator bool() const {
		return valid();
	}


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

	UniqueEntityPtr(EntityMgr* mgr, handle64 entity) noexcept
		: ptr(mgr, entity) {
	}

	UniqueEntityPtr(const EntityPtr& entity) noexcept
		: ptr(entity) {
	}

	UniqueEntityPtr(const UniqueEntityPtr& ptr) = delete;
	UniqueEntityPtr(UniqueEntityPtr&& ptr) noexcept = default;


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
	
	UniqueEntityPtr& operator=(UniqueEntityPtr&& entity_ptr) noexcept {
		if (ptr.valid()) reset();
		this->ptr = std::move(entity_ptr.ptr);
		return *this;
	}

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
	IEntity* operator->() const {
		return ptr.operator->();
	}

	[[nodiscard]]
	IEntity& operator*() const {
		return ptr.operator*();
	}

	[[nodiscard]]
	operator bool() const {
		return ptr.operator bool();
	}


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	IEntity* get() const {
		return ptr.get();
	}

	[[nodiscard]]
	EntityPtr getPtr() const noexcept {
		return ptr;
	}

	[[nodiscard]]
	handle64 getHandle() const noexcept {
		return ptr.getHandle();
	}

	void reset();


private:
	EntityPtr ptr;
};
*/