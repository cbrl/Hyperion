module;

#include <array>
#include <functional>
#include <vector>

#include "datatypes/scalar_types.h"

export module ecs:event_dispatcher;

export namespace ecs {

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
	void enqueue(ArgsT&&... args) {
		events[current_queue].emplace_back(std::forward<ArgsT>(args)...);
	}

	// Immediately send an event to all registered callbacks
	template<typename... ArgsT>
	void send(ArgsT&&... args) {
		// Construct the event
		const auto event = EventT{std::forward<ArgsT>(args)...};

		// Send the event
		for (auto& callback : event_callbacks) {
			callback(event);
		}
	}

	// Send events to all listeners
	void dispatch() override {
		// Update current_queue before processing events so that events sent during
		// event processing will be placed in the next queue.
		const u8 proc_queue = current_queue;
		current_queue = (current_queue + 1) % events.size();

		// Dispatch events
		for (auto& callback : event_callbacks) {
			for (auto& event : events[proc_queue]) {
				callback(event);
			}
		}
		events[proc_queue].clear();
	}

	// Add a free function callback
	template<auto Function>
	DispatcherConnection addCallback() {
		const auto func = std::function<void(const EventT&)>{Function};
		return addCallback(func);
	}

	// Add a class member function callback
	template<auto Function, typename ClassT>
	DispatcherConnection addCallback(ClassT* instance) {
		const auto func = std::function<void(const EventT&)>{std::bind(Function, instance, std::placeholders::_1)};
		return addCallback(func);
	}

	// Remove a free function callback
	template<auto Function>
	void removeCallback() {
		const auto func = std::function<void(const EventT&)>{ Function };
		removeCallback(func);
	}

	// Remove a class member function callback
	template<auto Function, typename ClassT>
	void removeCallback(ClassT* instance) {
		const auto func = std::function<void(const EventT&)>{std::bind(Function, instance, std::placeholders::_1)};
		removeCallback(func);
	}

	// Get the number of callbacks in this dispatcher
	[[nodiscard]]
	size_t getCallbackCount() const noexcept override {
		return event_callbacks.size();
	}

	
private:

	DispatcherConnection addCallback(const std::function<void(const EventT&)>& callback) {
		if (!callback) return {};

		// Search for an existing callback of the same type
		auto it = std::find_if(
			event_callbacks.begin(),
			event_callbacks.end(),
			[&](const std::function<void(const EventT&)>& other) {
				return other.target_type() == callback.target_type();
			}
		);

		// Add the callback if it doesn't exist
		if (it == event_callbacks.end()) {
			event_callbacks.push_back(callback);
			it = event_callbacks.begin() + (event_callbacks.size() - 1);
		}

		return DispatcherConnection{ [this, func = *it] {this->removeCallback(func); } };
	}

	void removeCallback(const std::function<void(const EventT&)>& callback){
		if (!callback) return;

		event_callbacks.erase(
			std::remove_if(
				event_callbacks.begin(),
				event_callbacks.end(),
				[&callback](const std::function<void(const EventT&)>& other) {
					return other.target_type() == callback.target_type();
				}),
			event_callbacks.end()
		);
	}

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::vector<std::function<void(const EventT&)>> event_callbacks;

	std::array<std::vector<EventT>, 2> events;
	u8 current_queue = 0;
};

} // namespace ecs
