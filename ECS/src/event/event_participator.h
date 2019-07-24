#pragma once

#include "datatypes/datatypes.h"


namespace ecs {

class EventMgr;
class DispatcherConnection;

//----------------------------------------------------------------------------------
// EventParticipator
//----------------------------------------------------------------------------------
//
// The EventSender and EventListener classes inherit from this class. It simply
// stores a pointer to the event manager, which should never be null. The pointer is
// used instead of a reference so that it can be set after construction, eliminating
// the need to modify the constructor of every class that uses events. The downside
// is that components/systems cannot call enqueue() in a constructor.
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
	// reference is assigned after construction, so this function cannot be called in a
	// constructor of a child class.
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
// User components/systems that send events will inherit from this class. The Entity
// class also inherits from this to notify systems when its parent changes.
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

	// Enqueue an event to be dispatched to all listeners
	template<typename EventT, typename... ArgsT>
	void enqueue(ArgsT&&... args);

	// Send an event to all listeners
	template<typename EventT, typename... ArgsT>
	void send(ArgsT&& ... args);

private:

	// Make getEventMgr() private
	using EventParticipator::getEventMgr;
};



//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------
//
// Components/Systems that listen for events will inherit from this class.
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

	// A container of registered connections. Used during unregisterAllEventCallbacks().
	std::vector<DispatcherConnection> connections;
};

} // namespace ecs

#include "event_participator.tpp"