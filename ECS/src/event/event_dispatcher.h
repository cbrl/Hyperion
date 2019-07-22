#pragma once

#include "datatypes/datatypes.h"
#include <functional>


namespace ecs {

class DispatcherConnection final {
	template<typename>
	friend class EventDispatcher;

	DispatcherConnection(const std::function<void()>& func)
		: disconnect_func(func) {
	}

public:
	DispatcherConnection() = default;
	DispatcherConnection(const DispatcherConnection&) = default;
	DispatcherConnection(DispatcherConnection&&) = default;

	~DispatcherConnection() = default;

	DispatcherConnection& operator=(const DispatcherConnection&) = default;
	DispatcherConnection& operator=(DispatcherConnection&&) = default;

	void disconnect() {
		if (disconnect_func) {
			disconnect_func();
			disconnect_func = nullptr;
		}
	}

private:
	std::function<void()> disconnect_func;
};


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
	virtual void dispatch() = 0;

	[[nodiscard]]
	virtual size_t getCallbackCount() const noexcept = 0;
};



//----------------------------------------------------------------------------------
// EvenDispatcher
//----------------------------------------------------------------------------------
//
// Stores EventDelegates that listen for events of type EventT, and dispatches
// events to them.
//
//----------------------------------------------------------------------------------
template<typename EventT>
class EventDispatcher final : public IEventDispatcher {
public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EventDispatcher() = default;
	EventDispatcher(const EventDispatcher&) = delete;
	EventDispatcher(EventDispatcher&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EventDispatcher() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventDispatcher& operator=(const EventDispatcher&) = delete;
	EventDispatcher& operator=(EventDispatcher&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	
	// Enqueue an event to be sent to all registered callbacks
	template<typename... ArgsT>
	void enqueue(ArgsT&&... args);

	// Immediately send an event to all registered callbacks
	template<typename... ArgsT>
	void send(ArgsT&&... args);

	// Send events to all listeners
	void dispatch() override;

	// Add an event callback to this dispatcher
	DispatcherConnection addCallback(const std::function<void(const EventT&)>& callback);

	// Remove an event callback from this dispatcher
	void removeCallback(const std::function<void(const EventT&)>& callback);

	// Get the number of callbacks in this dispatcher
	[[nodiscard]]
	size_t getCallbackCount() const noexcept override;

	
private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::vector<std::function<void(const EventT&)>> event_callbacks;

	std::array<std::vector<EventT>, 2> events;
	u8 current_queue = 0;
};

} // namespace ecs

#include "event_dispatcher.tpp"