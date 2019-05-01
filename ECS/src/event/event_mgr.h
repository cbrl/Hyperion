#pragma once

#include "event/event.h"
#include "event/event_dispatcher.h"

#include "datatypes/datatypes.h"
#include "log/log.h"
#include <memory_resource>


namespace ecs {

class EventMgr final {
	friend class EventListener;

public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventMgr() = default;
	EventMgr(const EventMgr&) = delete;
	EventMgr(EventMgr&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EventMgr() = default;
	

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventMgr& operator=(const EventMgr&) = delete;
	EventMgr& operator=(EventMgr&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Reset the event buffer
	void clearEventBuffer();

	// Erase all event dispatchers
	void clearEventDispatchers();

	// Queue an event to be dispatched to all relevant listeners
	template<class EventT, class... ArgsT>
	void send(ArgsT&&... args);

	// Dispatch all stored events and clear the event buffer
	void dispatchEvents();

private:

	// Add an event callback to the relevant dispatcher. The dispatcher will own the delegate.
	template<class EventT>
	void addEventCallback(std::unique_ptr<IEventDelegate> delegate);

	// Remove an event callback
	void removeEventCallback(gsl::not_null<IEventDelegate*> delegate);


private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unordered_map<std::type_index, std::unique_ptr<IEventDispatcher>> event_dispatchers;

	// Holds a list of all events sent since the last dispatchEvents() call
	std::vector<IEvent*> events;

	// The memory resource that allocates memory for events
	std::pmr::synchronized_pool_resource event_pool;
};

} // namespace ecs

#include "event_mgr.tpp"