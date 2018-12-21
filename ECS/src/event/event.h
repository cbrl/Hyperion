#pragma once

#include <typeindex>
#include <chrono>


class IEvent {
	using high_res_clk = std::chrono::high_resolution_clock;

public:

	IEvent() {
		creation_time = high_res_clk::now();
	}

	virtual ~IEvent() = default;
	
	virtual std::type_index getID() const noexcept = 0;
	
	const high_res_clk::time_point& getCreationTime() const noexcept {
		return creation_time;
	}

private:

	high_res_clk::time_point creation_time;
};


template<typename T>
class Event : public IEvent {
public:
	Event() = default;
	virtual ~Event() = default;

	std::type_index getID() const noexcept override final {
		return static_index;
	}

public:
	// won't be part of stored event memory
	static const std::type_index static_index;
};

template<typename T>
const std::type_index Event<T>::static_index = std::type_index{typeid(T)};