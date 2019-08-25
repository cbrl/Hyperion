#pragma once

#include "datatypes/types.h"
#include "log/log.h"
#include <memory_resource>


namespace ecs {

class IEventDispatcher;

template<typename EventT>
class EventDispatcher;

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
	void clearDispatchers();

	// Queue an event to be dispatched to all relevant listeners
	template<typename EventT, typename... ArgsT>
	void enqueue(ArgsT&&... args);

	// Immediately send an event to all relevant listeners
	template<typename EventT, typename... ArgsT>
	void send(ArgsT&&... args);

	// Dispatch all stored events and clear the event buffer
	void dispatch();

	// Get the dispatcher for the specified event type
	template<typename EventT>
	[[nodiscard]]
	EventDispatcher<EventT>& getDispatcher();

	// Remove an event callback
	template<typename EventT>
	void removeCallback(const std::function<void(const EventT&)>& callback);


private:

	template<typename EventT>
	[[nodiscard]]
	EventDispatcher<EventT>& getOrCreateDispatcher();

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unordered_map<std::type_index, std::unique_ptr<IEventDispatcher>> event_dispatchers;
};

} // namespace ecs

#include "event_mgr.tpp"