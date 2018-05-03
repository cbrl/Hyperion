#pragma once

#include "util\datatypes\datatypes.h"


class ISystem {
	friend class SystemMgr;

	public:
		ISystem() = default;
		virtual ~ISystem() = default;

		virtual type_index GetTypeID() const = 0;

		void SetActive(bool state) {
			active = state;
		}

		bool IsActive() const {
			return active;
		}

		virtual void PreUpdate(float dt) = 0;
		virtual void Update(float dt) = 0;
		virtual void PostUpdate(float dt) = 0;

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


	public:
		// An ID unique to type T
		static const type_index type_id;
};


template<typename T>
const type_index System<T>::type_id = type_index(typeid(T));