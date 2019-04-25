#pragma once

#include "event/event_delegate.h"
#include "datatypes/datatypes.h"


class EventMgr;


//----------------------------------------------------------------------------------
// EventParticipator
//----------------------------------------------------------------------------------
//
// The EventSender and EventListener classes inherit from this class. It contains
// a pointer to the event manager, which should never be null. The pointer is used
// instead of a reference so that it can be set after construction in the
// entity/component/system manager, eliminating the need to modify the constructor
// of every component or system.
//
//----------------------------------------------------------------------------------
class EventParticipator {
	friend class EntityMgr;
	friend class ComponentMgr;
	friend class SystemMgr;

public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventParticipator() noexcept = default;
	EventParticipator(const EventParticipator&) noexcept = default;
	EventParticipator(EventParticipator&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~EventParticipator() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventParticipator& operator=(const EventParticipator&) noexcept = default;
	EventParticipator& operator=(EventParticipator&&) noexcept = default;

protected:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Get a reference to the event manager of the ECS this participator belongs to. The
	// reference is assigned after construction, so this function 
	[[nodiscard]]
	EventMgr& getEventMgr() const noexcept;

private:

	void setEventMgr(gsl::not_null<EventMgr*> handler) noexcept;


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	EventMgr* event_mgr = nullptr;
};



//----------------------------------------------------------------------------------
// EventSender
//----------------------------------------------------------------------------------
//
// Components/Systems that send events will inherit from this class. The Entity
// class also inherits from this.
//
//----------------------------------------------------------------------------------
class EventSender : public virtual EventParticipator {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventSender() noexcept = default;
	EventSender(const EventSender&) noexcept = default;
	EventSender(EventSender&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~EventSender() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventSender& operator=(const EventSender&) noexcept = default;
	EventSender& operator=(EventSender&&) noexcept = default;

protected:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Queue an event to be dispatched to all listeners
	template<typename EventT, typename... ArgsT>
	void sendEvent(ArgsT&&... args);

private:

	// Make getEventMgr() private
	using EventParticipator::getEventMgr;
};



//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------
//
// Components/Systems that listen for events will inherit from this class
//
//----------------------------------------------------------------------------------
class EventListener : public virtual EventParticipator {
	friend class ComponentMgr;
	friend class SystemMgr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventListener() = default;
	EventListener(const EventListener&) = delete;
	EventListener(EventListener&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	virtual ~EventListener();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventListener& operator=(const EventListener&) = delete;
	EventListener& operator=(EventListener&&) noexcept = default;

protected:

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Register event handling callback functions. Will be called automatically by the
	// component/system manager. This function should contain calls to registerEventCallback().
	virtual void registerCallbacks() = 0;

	// Register a single event handling callback function
	template<typename ClassT, typename EventT>
	void registerEventCallback(void (ClassT::*Callback)(const EventT&));

	// Unregister a single event handling callback function
	template<typename ClassT, typename EventT>
	void unregisterEventCallback(void (ClassT::*Callback)(const EventT&));

	// Unregister all callback functions
	void unregisterAllEventCallbacks();

private:

	// registerCallbacks() is a virtual function. The "friend" declaration doesn't apply to
	// overrided functions in derived classes, so SystemMgr/ComponentMgr can't call it. The
	// managers instead call this function, which in turn calls the virtual function.
	void doRegisterCallbacks();

	// Make getEventMgr() private
	using EventParticipator::getEventMgr;

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A list of callback delegates registered by this event listener. The event manager owns the delegates.
	std::list<IEventDelegate*> registered_callbacks;
};

#include "event_participator.tpp"
