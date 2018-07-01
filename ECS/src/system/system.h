#pragma once

#include "datatypes/datatypes.h"

class Engine;


class ISystem {
	friend class SystemMgr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ISystem() noexcept
		: active(true) {
	}

	ISystem(const ISystem& system) = delete;
	ISystem(ISystem&& system) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~ISystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ISystem& operator=(const ISystem& system) = delete;
	ISystem& operator=(ISystem&& system) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	virtual type_index getTypeId() const = 0;

	void setActive(bool state) {
		active = state;
	}

	bool isActive() const {
		return active;
	}

	virtual void preUpdate(const Engine& engine) = 0;
	virtual void update(const Engine& engine) = 0;
	virtual void postUpdate(const Engine& engine) = 0;

protected:
	// Is this system enabled?
	bool active;
};




template<typename T>
class System : public ISystem {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	System() = default;
	System(const System& system) = delete;
	System(System&& system) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~System() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	System& operator=(const System& system) = delete;
	System& operator=(System&& system) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	type_index getTypeId() const override {
		return type_id;
	}

	void preUpdate(const Engine& engine) override {
	};

	void update(const Engine& engine) override {
	};

	void postUpdate(const Engine& engine) override {
	};


public:
	// An ID unique to type T
	static const type_index type_id;
};


template<typename T>
const type_index System<T>::type_id = type_index(typeid(T));
