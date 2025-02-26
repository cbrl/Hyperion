module;

#include <memory>
#include <memory_resource>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

#include "datatypes/scalar_types.h"

export module ecs:event_mgr;

import log;
import :event_dispatcher;

export namespace ecs {

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
	void clearDispatchers() {
		event_dispatchers.clear();
	}

	// Queue an event to be dispatched to all relevant listeners
	template<typename EventT, typename... ArgsT>
	void enqueue(ArgsT&&... args) {
		const auto index = std::type_index{typeid(EventT)};

		if (const auto it = event_dispatchers.find(index); it != event_dispatchers.end()) {
			auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(it->second));
			dispatcher.enqueue(std::forward<ArgsT>(args)...);
		}
	}

	// Immediately send an event to all relevant listeners
	template<typename EventT, typename... ArgsT>
	void send(ArgsT&&... args) {
		const auto index = std::type_index{typeid(EventT)};

		if (const auto it = event_dispatchers.find(index); it != event_dispatchers.end()) {
			auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(it->second));
			dispatcher.send(std::forward<ArgsT>(args)...);
		}
	}

	// Dispatch all stored events and clear the event buffer
	void dispatch() {
		for (auto& [id, dispatcher] : event_dispatchers) {
			dispatcher->dispatch();
		}
	}

	// Get the dispatcher for the specified event type
	template<typename EventT>
	[[nodiscard]]
	EventDispatcher<EventT>& getDispatcher() {
		return getOrCreateDispatcher<EventT>();
	}

	// Remove an event callback
	template<typename EventT>
	void removeCallback(const std::function<void(const EventT&)>& callback){
		const auto index = std::type_index{typeid(EventT)};

		if (const auto it = event_dispatchers.find(index); it != event_dispatchers.end()) {
			it->second->removeCallback(callback);
		}
	}


private:

	template<typename EventT>
	[[nodiscard]]
	EventDispatcher<EventT>& getOrCreateDispatcher() {
		const std::type_index index{typeid(EventT)};
		const auto it = event_dispatchers.find(index);

		if (it == event_dispatchers.end()) {
			const auto [iter, inserted] = event_dispatchers.try_emplace(index, std::make_unique<EventDispatcher<EventT>>());
			return static_cast<EventDispatcher<EventT>&>(*(iter->second));
		}
		else {
			return static_cast<EventDispatcher<EventT>&>(*(it->second));
		}
	}

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unordered_map<std::type_index, std::unique_ptr<IEventDispatcher>> event_dispatchers;
};

} // namespace ecs
