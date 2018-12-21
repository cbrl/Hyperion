#pragma once

#include "event/event_handler.h"
#include "event/event_delegate.h"

#include "string/string.h"
#include "log/log.h"


class IEventDispatcher {
public:
	virtual ~IEventDispatcher() {}

	virtual void dispatch(IEvent* event) = 0;

	virtual void addEventCallback(IEventDelegate* const eventDelegate) = 0;

	virtual void removeEventCallback(IEventDelegate* eventDelegate) = 0;

	virtual inline size_t getEventCallbackCount() const = 0;
};


template<class T>
class EventDispatcher final : public IEventDispatcher {
public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	// never use!
	EventDispatcher()
		: locked(false) {
	}

	EventDispatcher(const EventDispatcher&) = delete;
	EventDispatcher(EventDispatcher&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~EventDispatcher() {
		//m_PendingAddDelegates.clear();
		pending_remove_delegates.clear();
		event_callbacks.clear();
	}


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EventDispatcher& operator=(const EventDispatcher&) = delete;
	EventDispatcher& operator=(EventDispatcher&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// send event to all listeners
	inline void dispatch(IEvent* event) override final {
		locked = true;

		{
			// remove pending delegates
			if (pending_remove_delegates.empty() == false) {
				for (auto iter : pending_remove_delegates) {
					event_callbacks.erase(iter);
				}
				pending_remove_delegates.clear();
			}

			for (auto& callback : event_callbacks) {
				assert(callback != nullptr && "Invalid event callback");
				callback->invoke(event);
			}
		}

		locked = false;
	}

	virtual void addEventCallback(IEventDelegate* const event_delegate) override final {
		// if delegate wasn't deleted since last update, that is, delegate is still in pending list,
		// remove it from pending list
		auto result = std::find_if(pending_remove_delegates.begin(), pending_remove_delegates.end(),
			[&](typename decltype(event_callbacks)::iterator& it) {
				return (*it)->operator==(*event_delegate);
			}
		);

		if (result != pending_remove_delegates.end()) {
			pending_remove_delegates.erase(result);
		}
		else {
			event_callbacks.emplace_back(event_delegate);
		}
	}

	virtual void removeEventCallback(IEventDelegate* event_delegate) override final { 

		auto result = std::find_if(event_callbacks.begin(), event_callbacks.end(),
			[&](std::unique_ptr<IEventDelegate>& other) {
				return other->operator==(*event_delegate);
			}
		);

		if (!locked && result != event_callbacks.end()) {
			event_callbacks.erase(result);
		}
		else {
			assert(result != event_callbacks.end() && "Invalid event callback specified for removal");
			pending_remove_delegates.push_back(result);
		}
	}

	virtual inline size_t getEventCallbackCount() const override final {
		return event_callbacks.size();
	}

	
private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	bool locked;

	std::list<std::unique_ptr<IEventDelegate>> event_callbacks;

	//std::list<IEventDelegate*> pending_add_delegates;
	std::list<typename decltype(event_callbacks)::iterator> pending_remove_delegates; //using iterators since std::list::iterator isn't invalidated
};
