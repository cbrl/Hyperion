#pragma once

#include "datatypes/datatypes.h"

class Engine;


class ISystem {
	friend class SystemMgr;

public:
	ISystem() : active(true) {
	};
	virtual ~ISystem() = default;

	virtual type_index GetTypeID() const = 0;

	void SetActive(bool state) {
		active = state;
	}

	bool IsActive() const {
		return active;
	}

	virtual void PreUpdate(const Engine& engine) = 0;
	virtual void Update(const Engine& engine) = 0;
	virtual void PostUpdate(const Engine& engine) = 0;

protected:
	// Is this system enabled?
	bool active;
};


template<typename T>
class System : public ISystem {
public:
	System() = default;
	virtual ~System() = default;

	type_index GetTypeID() const override {
		return type_id;
	}

	void PreUpdate(const Engine& engine) override {
	};

	void Update(const Engine& engine) override {
	};

	void PostUpdate(const Engine& engine) override {
	};


public:
	// An ID unique to type T
	static const type_index type_id;
};


template<typename T>
const type_index System<T>::type_id = type_index(typeid(T));
