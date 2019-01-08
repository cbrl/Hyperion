#pragma once

#include "event/event_mgr.h"
#include "event/event_delegate.h"

#include "string/string.h"
#include "log/log.h"


class IEventDispatcher {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	IEventDispatcher() noexcept = default;
	IEventDispatcher(const IEventDispatcher&) = delete;
	IEventDispatcher(IEventDispatcher&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	virtual ~IEventDispatcher() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	IEventDispatcher& operator=(const IEventDispatcher&) = delete;
	IEventDispatcher& operator=(IEventDispatcher&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	virtual void dispatch(IEvent* event) = 0;

	virtual void addEventCallback(IEventDelegate* const eventDelegate) = 0;

	virtual void removeEventCallback(IEventDelegate* eventDelegate) = 0;

	[[nodiscard]]
	virtual size_t getEventCallbackCount() const noexcept = 0;
};



//----------------------------------------------------------------------------------
// EvenDispatcher
//----------------------------------------------------------------------------------
//
// Holds EventDelegates which listen for events of type EventT, and dispatches
// events to them.
//
//----------------------------------------------------------------------------------
template<class EventT>
class EventDispatcher final : public IEventDispatcher {
public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventDispatcher();
	EventDispatcher(const EventDispatcher&) = delete;
	EventDispatcher(EventDispatcher&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EventDispatcher();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventDispatcher& operator=(const EventDispatcher&) = delete;
	EventDispatcher& operator=(EventDispatcher&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Send an event to all listeners
	void dispatch(IEvent* event) override final;

	// Add an event callback delegate to this dispatcher
	void addEventCallback(IEventDelegate* delegate) override final;

	// Remove an event callback delegate from this dispatcher
	void removeEventCallback(IEventDelegate* delegate) override final;

	// Get the number of callback delegates in this dispatcher
	[[nodiscard]]
	size_t getEventCallbackCount() const noexcept override final;

	
private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	bool delegate_list_locked;

	std::list<std::unique_ptr<IEventDelegate>> event_delegates;

	//std::list<IEventDelegate*> pending_add_delegates;
	std::list<typename decltype(event_delegates)::iterator> pending_remove_delegates; //using iterators since std::list::iterator isn't invalidated
};

#include "event_dispatcher.tpp"
