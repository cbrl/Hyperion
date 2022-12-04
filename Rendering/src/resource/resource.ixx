module;

#include "datatypes/types.h"
#include "io/io.h"

export module rendering:resource;


export namespace render {

template<typename ResourceT>
class Resource {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Resource(std::wstring_view guid)
	    : guid(WstrToStr(guid)) {
	}

	Resource(std::string guid)
	    : guid(std::move(guid)) {
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

	std::string guid;
};

} //namespace render
