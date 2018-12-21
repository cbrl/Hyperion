#pragma once

#include "system/system.h"
#include "exception/exception.h"


class EventMgr;
class EventListener;


class SystemMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	SystemMgr(EventMgr& handler);
	SystemMgr(const SystemMgr& manager) = delete;
	SystemMgr(SystemMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~SystemMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	SystemMgr& operator=(const SystemMgr& manager) = delete;
	SystemMgr& operator=(SystemMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void update(Engine& engine);

	template<typename SystemT, typename... ArgsT>
	SystemT* addSystem(ArgsT&&... args);

	template<typename SystemT>
	SystemT* getSystem() const;


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The systems, mapped to their type_index
	std::unordered_map<std::type_index, std::unique_ptr<ISystem>> systems;

	// A reference to the event manager. Passed to systems that inherit from EventListener.
	EventMgr& event_handler;
};

#include "system_mgr.tpp"
