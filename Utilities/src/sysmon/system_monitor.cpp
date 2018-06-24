#include "system_monitor.h"


//----------------------------------------------------------------------------------
//
// CPUMonitor
//
//----------------------------------------------------------------------------------

void SystemMonitor::CpuMonitor::tick() {

	// Update wall clock and elapsed time
	wall_timer.tick();
	dt += wall_timer.deltaTime();

	// Update stats only if 1/2 second has passed
	if (dt > 0.5) {

		// Update system clocks
		core_timer.tick();
		sys_work_timer.tick();
		sys_idle_timer.tick();

		// Update cpu stats
		sys_usage  = ((sys_work_timer.deltaTime() - sys_idle_timer.deltaTime()) * 100.0) / sys_work_timer.deltaTime();
		proc_usage = (core_timer.deltaTime() * 100.0) / dt;

		// Reset elapsed time
		dt = 0.0;
	}
}


[[nodiscard]]
f64 SystemMonitor::CpuMonitor::getTotalCpuPercentage() const {
	return sys_usage;
}


[[nodiscard]]
f64 SystemMonitor::CpuMonitor::getProcessCpuPercentage() const {
	return proc_usage;
}


//----------------------------------------------------------------------------------
//
// MemoryMonitor
//
//----------------------------------------------------------------------------------

void SystemMonitor::MemoryMonitor::tick() {

	// Total memory
	GlobalMemoryStatusEx(&mem_info);


	// Process memory
	GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
}


[[nodiscard]]
u64 SystemMonitor::MemoryMonitor::getPhysicalMemSize() const {
	return mem_info.ullTotalPhys;
}


[[nodiscard]]
u64 SystemMonitor::MemoryMonitor::getVirtualMemSize() const {
	return mem_info.ullTotalPageFile;
}


[[nodiscard]]
u64 SystemMonitor::MemoryMonitor::getTotalUsedPhysicalMem() const {
	return mem_info.ullTotalPhys - mem_info.ullAvailPhys;
}


[[nodiscard]]
u64 SystemMonitor::MemoryMonitor::getProcessUsedPhysicalMem() const {
	return pmc.WorkingSetSize;
}


[[nodiscard]]
u64 SystemMonitor::MemoryMonitor::getTotalUsedVirtualMem() const {
	return mem_info.ullTotalPageFile - mem_info.ullAvailPageFile;
}


[[nodiscard]]
u64 SystemMonitor::MemoryMonitor::getProcessUsedVirtualMem() const {
	return pmc.PrivateUsage;
}
