#pragma once

#include "event/event_delegate.h"
#include "datatypes/datatypes.h"


class EventHandler;


//----------------------------------------------------------------------------------
// EventParticipator
//----------------------------------------------------------------------------------
//
// The EventSender and EventListener classes inherit from this class. It contains
// a pointer to the event handler, which should never be null. The pointer is used
// instead of a reference so that it can be set after construction in the
// component/system manager, eliminating the need to modify the constructor of
// every component or system.
//
//----------------------------------------------------------------------------------
class EventParticipator {
public:
	void setEventHandler(gsl::not_null<EventHandler*> handler) noexcept;

protected:
	EventHandler* getEventHandler() const noexcept;

private:
	EventHandler* event_handler;
};



//----------------------------------------------------------------------------------
// EventSender
//----------------------------------------------------------------------------------
//
// Components/Systems that send events will inherit from this class
//
//----------------------------------------------------------------------------------
class EventSender : public virtual EventParticipator {
protected:

	// Queue an event to be dispatched to all listeners
	template<typename EventT, typename... ArgsT>
	void sendEvent(ArgsT&&... args);
};



//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------
//
// Components/Systems that listen for events will inherit from this class
//
//----------------------------------------------------------------------------------
class EventListener : public virtual EventParticipator {
public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventListener() = default;
	EventListener(const EventListener&) = delete;
	EventListener(EventListener&&) = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	virtual ~EventListener();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventListener& operator=(const EventListener&) = delete;
	EventListener& operator=(EventListener&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Register event handling callback functions. Will be called automatically by the
	// component/system manager. This function should contain calls to registerEventCallback().
	virtual void registerCallbacks() = 0;

protected:

	// Register a single event handling callback function
	template<typename ClassT, typename EventT>
	void registerEventCallback(void (ClassT::*Callback)(const EventT* const));

	// Unregister a single event handling callback function
	template<typename ClassT, typename EventT>
	void unregisterEventCallback(void (ClassT::*Callback)(const EventT* const));

	// Unregister all callback functions
	void unregisterAllEventCallbacks();


private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A list of callback delegates registered by this event listener. The event handler owns the delegates.
	std::list<IEventDelegate*> registered_callbacks;
};

#include "event_participator.tpp"
