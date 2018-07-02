#pragma once

#include "datatypes/datatypes.h"
#include "io/io.h"


template<typename ResourceT>
class Resource {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Resource(const wstring& guid)
		: guid(guid) {
	}

	Resource(wstring&& guid)
		: guid(guid) {
	}

	Resource(const Resource& resource) = delete;
	Resource(Resource&& resource) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~Resource() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Resource& operator=(const Resource& resource) = delete;
	Resource& operator=(Resource&& resource) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const wstring& getGUID() const {
		return guid;
	}

	[[nodiscard]]
	bool isFileGUID() const {
		return fs::is_regular_file(guid);
	}


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	wstring guid;
};
