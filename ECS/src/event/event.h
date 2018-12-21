#pragma once

#include <typeindex>
#include <chrono>


class IEvent {
	using high_res_clk = std::chrono::high_resolution_clock;

public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	IEvent() : creation_time(high_res_clk::now()) {}

	IEvent(const IEvent&) noexcept = default;
	IEvent(IEvent&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	virtual ~IEvent() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	IEvent& operator=(const IEvent&) noexcept = default;
	IEvent& operator=(IEvent&&) noexcept = default;
	

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	virtual std::type_index getID() const noexcept = 0;
	
	const high_res_clk::time_point& getCreationTime() const noexcept {
		return creation_time;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	high_res_clk::time_point creation_time;
};



//----------------------------------------------------------------------------------
// Event
//----------------------------------------------------------------------------------
//
// All events will inherit from this class. Events should contain any relevant
// data needed to process the event.
//
//----------------------------------------------------------------------------------
template<typename T>
class Event : public IEvent {
public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Event() = default;
	Event(const Event&) noexcept = default;
	Event(Event&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	virtual ~Event() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Event& operator=(const Event&) noexcept = default;
	Event& operator=(Event&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	std::type_index getID() const noexcept override final {
		return static_index;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	static const std::type_index static_index;
};

template<typename T>
const std::type_index Event<T>::static_index = std::type_index{typeid(T)};