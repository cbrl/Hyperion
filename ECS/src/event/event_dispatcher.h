#pragma once

#include "event/event_handler.h"
#include "event/event_delegate.h"

#include "string/string.h"
#include "log/log.h"


class IEventDispatcher {
public:
	virtual ~IEventDispatcher() {}

	virtual void dispatch(IEvent* event) = 0;

	virtual void addEventCallback(IEventDelegate* const eventDelegate) = 0;

	virtual void removeEventCallback(IEventDelegate* eventDelegate) = 0;

	virtual size_t getEventCallbackCount() const = 0;
};


template<class T>
class EventDispatcher final : public IEventDispatcher {
public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventDispatcher() : locked(false) {}
	EventDispatcher(const EventDispatcher&) = delete;
	EventDispatcher(EventDispatcher&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EventDispatcher() {
		//m_PendingAddDelegates.clear();
		pending_remove_delegates.clear();
		event_callbacks.clear();
	}


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventDispatcher& operator=(const EventDispatcher&) = delete;
	EventDispatcher& operator=(EventDispatcher&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// send event to all listeners
	void dispatch(IEvent* event) override final;

	void addEventCallback(IEventDelegate* delegate) override final;

	void removeEventCallback(IEventDelegate* delegate) override final;

	size_t getEventCallbackCount() const override final;

	
private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	bool locked;

	std::list<std::unique_ptr<IEventDelegate>> event_callbacks;

	//std::list<IEventDelegate*> pending_add_delegates;
	std::list<typename decltype(event_callbacks)::iterator> pending_remove_delegates; //using iterators since std::list::iterator isn't invalidated
};

#include "event_dispatcher.tpp"
