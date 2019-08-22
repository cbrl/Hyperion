#pragma once

#include "datatypes/datatypes.h"
#include <functional>


namespace ecs {

class DispatcherConnection {
	template<typename>
	friend class EventDispatcher;

	DispatcherConnection(const std::function<void()>& func)
		: disconnect_func(func) {
	}

public:

	DispatcherConnection() = default;
	DispatcherConnection(const DispatcherConnection&) = default;
	DispatcherConnection(DispatcherConnection&& other) noexcept
		: disconnect_func(std::exchange(other.disconnect_func, nullptr)) {
	}

	~DispatcherConnection() = default;

	DispatcherConnection& operator=(const DispatcherConnection&) = default;
	DispatcherConnection& operator=(DispatcherConnection&& other) noexcept {
		this->disconnect_func = std::exchange(other.disconnect_func, nullptr);
		return *this;
	}

	void disconnect() {
		if (disconnect_func) {
			disconnect_func();
			disconnect_func = nullptr;
		}
	}

private:

	std::function<void()> disconnect_func;
};


class UniqueDispatcherConnection : private DispatcherConnection {
public:
	UniqueDispatcherConnection() = default;
	UniqueDispatcherConnection(const DispatcherConnection& connection)
		: DispatcherConnection(connection) {
	}
	UniqueDispatcherConnection(const UniqueDispatcherConnection&) = delete;
	UniqueDispatcherConnection(UniqueDispatcherConnection&&) noexcept = default;

	~UniqueDispatcherConnection() {
		disconnect();
	}

	UniqueDispatcherConnection& operator=(const UniqueDispatcherConnection&) = delete;
	UniqueDispatcherConnection& operator=(UniqueDispatcherConnection&&) noexcept = default;

	using DispatcherConnection::disconnect;
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

	// Add a free function callback
	template<auto Function>
	DispatcherConnection addCallback();

	// Add a class member function callback
	template<auto Function, typename ClassT>
	DispatcherConnection addCallback(ClassT* instance);

	// Remove a free function callback
	template<auto Function>
	void removeCallback();

	// Remove a class member function callback
	template<auto Function, typename ClassT>
	void removeCallback(ClassT* instance);

	// Get the number of callbacks in this dispatcher
	[[nodiscard]]
	size_t getCallbackCount() const noexcept override;

	
private:

	DispatcherConnection addCallback(const std::function<void(const EventT&)>& callback);

	void removeCallback(const std::function<void(const EventT&)>& callback);

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::vector<std::function<void(const EventT&)>> event_callbacks;

	std::array<std::vector<EventT>, 2> events;
	u8 current_queue = 0;
};

} // namespace ecs

#include "event_dispatcher.tpp"