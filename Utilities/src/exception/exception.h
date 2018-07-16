#pragma once

#include <cassert>
#include <exception>
#include <iostream>


class Exception : public virtual std::exception {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Exception() : msg{} {
	}

	Exception(const char* message)
		: msg(message) {
	}

	Exception(const Exception& exc) = default;
	Exception(Exception&& exc) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Exception() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Exception& operator=(const Exception& exc) = default;
	Exception& operator=(Exception&& exc) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	const char* what() const noexcept override {
		return msg;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	const char* msg;
};


// Throw and log if the condition fails
inline void ThrowIfFailed(bool result, const char* msg = "") {
	if (!result) {
		std::cerr << msg << std::endl;
		throw Exception(msg);
	}
}