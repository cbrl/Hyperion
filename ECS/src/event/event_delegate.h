#pragma once

#include <typeindex>


class IEvent;


class IEventDelegate {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	IEventDelegate() noexcept = default;
	IEventDelegate(const IEventDelegate&) noexcept = default;
	IEventDelegate(IEventDelegate&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~IEventDelegate() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	IEventDelegate& operator=(const IEventDelegate&) noexcept = default;
	IEventDelegate& operator=(IEventDelegate&&) noexcept = default;

	virtual bool operator==(const IEventDelegate& other) const = 0;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	virtual void invoke(const IEvent* e) = 0;

	[[nodiscard]]
	virtual size_t getDelegateID() const noexcept = 0;

	[[nodiscard]]
	virtual std::type_index getEventID() const noexcept = 0;
};



//----------------------------------------------------------------------------------
// EventDelegate
//----------------------------------------------------------------------------------
//
// Holds a pointer to a member function in class ClassT which listens for events
// of type EventT. When an applicable event is sent, the function will be invoked
// with a pointer to the event.
//
//----------------------------------------------------------------------------------
template<typename ClassT, typename EventT>
class EventDelegate final : public IEventDelegate {
	using callback = void (ClassT::*)(const EventT*);

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventDelegate(ClassT* receiver, callback function)
		: receiver(receiver)
	    , function(function) {
	}

	EventDelegate(const EventDelegate&) noexcept = default;
	EventDelegate(EventDelegate&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EventDelegate() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventDelegate& operator=(const EventDelegate&) noexcept = default;
	EventDelegate& operator=(EventDelegate&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void invoke(const IEvent* e) override final {
		std::invoke(function, receiver, reinterpret_cast<const EventT*>(e));
	}

	[[nodiscard]]
	size_t getDelegateID() const noexcept override final {
		return delegate_index;
	}

	[[nodiscard]]
	std::type_index getEventID() const noexcept override final {
		return EventT::static_index;
	}

	[[nodiscard]]
	bool operator==(const IEventDelegate& other) const noexcept override final {
		if (this->getDelegateID() != other.getDelegateID())
			return false;

		return (this->function == static_cast<const EventDelegate&>(other).function &&
		        this->receiver == static_cast<const EventDelegate&>(other).receiver);
	}


private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ClassT*  receiver;
	callback function;

	static const size_t delegate_index;
};

template<typename ClassT, typename EventT>
const size_t EventDelegate<ClassT, EventT>::delegate_index = typeid(ClassT).hash_code() ^ typeid(callback).hash_code();