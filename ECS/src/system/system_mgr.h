#pragma once

#include "system/system.h"
#include "exception/exception.h"


class SystemMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	SystemMgr() = default;
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
	SystemT* addSystem(ArgsT&&... args) {
		const auto& it = systems.find(SystemT::index);

		if (it != systems.end() && it->second != nullptr)
			return static_cast<SystemT*>(it->second.get());

		auto pair = systems.emplace(SystemT::index, new SystemT(std::forward<ArgsT>(args)...));
		return static_cast<SystemT*>(pair.first->second.get());
	}

	template<typename SystemT>
	SystemT* getSystem() const {
		const auto& it = systems.find(SystemT::index);

		if (it != systems.end() && it->second != nullptr)
			return static_cast<SystemT*>(it->second.get());

		return nullptr;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The systems, mapped to their type_index
	std::unordered_map<std::type_index, unique_ptr<ISystem>> systems;
};
