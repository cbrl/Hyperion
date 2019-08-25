#pragma once

#include "datatypes/types.h"
#include "io/io.h"


namespace render {

template<typename ResourceT>
class Resource {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Resource(const std::wstring& guid)
	    : str_guid{WstrToStr(guid)}
		, wstr_guid{guid} {
	}

	Resource(const std::string& guid)
	    : str_guid{guid}
	    , wstr_guid{StrToWstr(guid)} {
	}

	Resource(const Resource& resource) = default;
	Resource(Resource&& resource)  = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~Resource() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Resource& operator=(const Resource& resource) = default;
	Resource& operator=(Resource&& resource) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const std::string& getGUID() const noexcept {
		return str_guid;
	}

	[[nodiscard]]
	const std::wstring& getWGUID() const noexcept {
		return wstr_guid;
	}

	[[nodiscard]]
	bool isFileGUID() const {
		return fs::is_regular_file(str_guid);
	}


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::string  str_guid;
	std::wstring wstr_guid;
};

} //namespace render