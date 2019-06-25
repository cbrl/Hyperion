#pragma once

#include "system/system.h"
#include "exception/exception.h"


namespace ecs {

class ECS;
class EventMgr;
class EventListener;

class SystemMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SystemMgr(ECS& ecs);
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

	void update(std::chrono::duration<f64> dt);

	template<typename SystemT, typename... ArgsT>
	SystemT& addSystem(ArgsT&&... args);

	void removeSystem(ISystem& system);

	template<typename SystemT>
	void removeSystem();

	template<typename SystemT>
	[[nodiscard]]
	SystemT& getSystem();

	template<typename SystemT>
	[[nodiscard]]
	const SystemT& getSystem() const;

	template<typename SystemT>
	[[nodiscard]]
	SystemT* tryGetSystem();

	template<typename SystemT>
	[[nodiscard]]
	const SystemT* tryGetSystem() const;

	// Set a system's priority. Higher priority systems get executed sooner.
	template<typename SystemT>
	void setSystemPriority(u32 priority);

private:

	void sortSystemQueue();

	struct PriorityCompareGreater {
		bool operator()(const ISystem& lhs, const ISystem& rhs) const {
			return lhs.getPriority() > rhs.getPriority();
		}
	};

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The systems, mapped to their type_index
	std::unordered_map<std::type_index, std::unique_ptr<ISystem>> systems;

	// The system execution order, determined by the system's priority.
	std::vector<std::reference_wrapper<ISystem>> system_queue;

	// A reference to the ECS. Passed to all systems.
	std::reference_wrapper<ECS> ecs;
};

} // namespace ecs

#include "system_mgr.tpp"