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




inline void ThrowIfFailed(bool result, const char* msg = "") {
	if (!result) {
		std::cerr << msg << std::endl;
		throw Exception(msg);
	}
}


#ifdef WIN32
inline void ThrowIfFailed(HRESULT hr, const char* msg = "") {
	if (FAILED(hr)) {
		std::cerr << msg << " (Failure with HRESULT of 0x" << std::hex << hr << std::dec << ")\n";
		throw Exception(msg);
	}
}
#endif