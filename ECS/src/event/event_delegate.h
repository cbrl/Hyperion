#pragma once

#include <typeindex>


class IEvent;


class IEventDelegate {
public:
	virtual ~IEventDelegate() = default;

	virtual void invoke(const IEvent* e) = 0;

	virtual size_t getDelegateID() const = 0;

	virtual std::type_index getEventID() const = 0;

	virtual bool operator==(const IEventDelegate& other) const = 0;
};


template<typename ClassT, typename EventT>
class EventDelegate final : public IEventDelegate {
	using callback = void (ClassT::*)(const EventT*);

public:

	EventDelegate(ClassT* receiver, callback function)
		: receiver(receiver)
	    , function(function) {
	}

	void invoke(const IEvent* e) override final {
		std::invoke(function, receiver, reinterpret_cast<const EventT*>(e));
	}

	size_t getDelegateID() const override final {
		return delegate_index;
	}

	std::type_index getEventID() const override final {
		return EventT::static_index;
	}

	bool operator==(const IEventDelegate& other) const override final {
		if (this->getDelegateID() != other.getDelegateID())
			return false;

		return (this->function == static_cast<const EventDelegate&>(other).function &&
		        this->receiver == static_cast<const EventDelegate&>(other).receiver);
	}


private:
	ClassT* receiver;
	callback function;

	static const size_t delegate_index;
};

template<typename ClassT, typename EventT>
const size_t EventDelegate<ClassT, EventT>::delegate_index = typeid(ClassT).hash_code() ^ typeid(callback).hash_code();