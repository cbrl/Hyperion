#pragma once

#include "event/event_mgr.h"
#include "event/event_delegate.h"

#include "string/string.h"
#include "log/log.h"


namespace ecs {

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
	virtual void dispatch(const IEvent& event) = 0;

	virtual void addEventCallback(std::unique_ptr<IEventDelegate> delegate) = 0;

	virtual void removeEventCallback(gsl::not_null<IEventDelegate*> delegate) = 0;

	[[nodiscard]]
	virtual size_t getEventCallbackCount() const noexcept = 0;
};



//----------------------------------------------------------------------------------
// EvenDispatcher
//----------------------------------------------------------------------------------
//
// Stores EventDelegates that listen for events of type EventT, and dispatches
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
	void dispatch(const IEvent& event) override;

	// Add an event callback delegate to this dispatcher. The dispatcher will own the delegate.
	void addEventCallback(std::unique_ptr<IEventDelegate> delegate) override;

	// Remove an event callback delegate from this dispatcher
	void removeEventCallback(gsl::not_null<IEventDelegate*> delegate) override;

	// Get the number of callback delegates in this dispatcher
	[[nodiscard]]
	size_t getEventCallbackCount() const noexcept override;

	
private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	bool delegate_list_locked;

	std::list<std::unique_ptr<IEventDelegate>> event_delegates;

	//std::list<IEventDelegate*> pending_add_delegates;
	std::list<typename decltype(event_delegates)::iterator> pending_remove_delegates; //using iterators since std::list::iterator isn't invalidated
};

} // namespace ecs

#include "event_dispatcher.tpp"