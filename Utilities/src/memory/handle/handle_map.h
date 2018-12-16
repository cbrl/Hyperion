#pragma once

#include "handle.h"
#include "exception/exception.h"
#include "datatypes/container_types.h"


template<typename HandleT, typename DataT, size_t chunk_size = 512>
class HandleMap {

	static_assert(std::is_pointer_v<DataT>,
	              "HandleMap: invalid DataT. The HandleMap does not store resources, only pointers to a resources.");

private:
	using table_entry = std::pair<typename HandleT::value_type, DataT>;


public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	HandleMap() noexcept {
		allocateChunk();
	}

	HandleMap(const HandleMap& map) = delete;
	HandleMap(HandleMap&& map) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~HandleMap() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	HandleMap& operator=(const HandleMap& map) = delete;
	HandleMap& operator=(HandleMap&& map) noexcept = default;

	[[nodiscard]] DataT operator[](HandleT handle);


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	HandleT createHandle(DataT object);

	void releaseHandle(HandleT handle);

	bool validHandle(const HandleT& handle) const noexcept;


private:
	bool allocateChunk();


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::vector<table_entry> table;
};

#include "handle_map.tpp"
