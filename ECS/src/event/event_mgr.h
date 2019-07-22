#pragma once

#include "datatypes/datatypes.h"
#include "log/log.h"
#include <memory_resource>


namespace ecs {

class IEvent;
class IEventDispatcher;
class DispatcherConnection;

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

	// Erase all event dispatchers
	void clearEventDispatchers();

	// Queue an event to be dispatched to all relevant listeners
	template<typename EventT, typename... ArgsT>
	void send(ArgsT&&... args);

	// Dispatch all stored events and clear the event buffer
	void dispatchEvents();

private:

	// Add an event callback to the relevant dispatcher. The dispatcher will own the delegate.
	template<typename EventT>
	DispatcherConnection addEventCallback(const std::function<void(const EventT&)>& callback);

	// Remove an event callback
	template<typename EventT>
	void removeEventCallback(const std::function<void(const EventT&)>& callback);


private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unordered_map<std::type_index, std::unique_ptr<IEventDispatcher>> event_dispatchers;
};

} // namespace ecs

#include "event_mgr.tpp"