#include "stdafx.h"
#include "system_monitor.h"


//----------------------------------------------------------------------------------
//
// CPUMonitor
//
//----------------------------------------------------------------------------------

SystemMonitor::CPUMonitor::~CPUMonitor() {
	if (can_read_cpu)
		PdhCloseQuery(query_handle);
}


void SystemMonitor::CPUMonitor::Init(HANDLE handle) {

	process_handle = handle;

	//----------------------------------------------------------------------------------
	// Initialize data used for total CPU usage
	//----------------------------------------------------------------------------------

	PDH_STATUS status;

	can_read_cpu = true;

	// Create query object to poll cpu
	status = PdhOpenQuery(NULL, NULL, &query_handle);
	if (status != ERROR_SUCCESS) {
		can_read_cpu = false;
	}

	// Set query object to poll all cpus
	status = PdhAddCounter(query_handle, TEXT("\\Processor(_Total)\\% Processor Time"), NULL, &counter_handle);
	if (status != ERROR_SUCCESS) {
		can_read_cpu = false;
	}

	last_sample_time = GetTickCount();
	total_usage = 0;



	//----------------------------------------------------------------------------------
	// Initialize data used for process CPU usage
	//----------------------------------------------------------------------------------

	SYSTEM_INFO sys_info;

	GetSystemInfo(&sys_info);
	processor_count = sys_info.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&last_cpu, &ftime, sizeof(FILETIME));

	GetProcessTimes(process_handle, &ftime, &ftime, &fsys, &fuser);
	memcpy(&last_sys_cpu, &fsys, sizeof(FILETIME));
	memcpy(&last_user_cpu, &fuser, sizeof(FILETIME));
}


void SystemMonitor::CPUMonitor::Tick() {

	//----------------------------------------------------------------------------------
	// Get total CPU usage
	//----------------------------------------------------------------------------------
	PDH_FMT_COUNTERVALUE value;

	if (can_read_cpu) {
		if ((last_sample_time + 1000) < GetTickCount()) {

			last_sample_time = GetTickCount();

			PdhCollectQueryData(query_handle);
			PdhGetFormattedCounterValue(counter_handle, PDH_FMT_LONG, NULL, &value);

			total_usage = value.longValue;
		}
	}



	//----------------------------------------------------------------------------------
	// Calculate percent CPU used by current process
	//----------------------------------------------------------------------------------

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(process_handle, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));

	process_usage  = static_cast<double>((sys.QuadPart - last_sys_cpu.QuadPart) + (user.QuadPart - last_user_cpu.QuadPart));
	process_usage /= (now.QuadPart - last_cpu.QuadPart);
	process_usage /= processor_count;

	last_cpu = now;
	last_user_cpu = user;
	last_sys_cpu = sys;
}


u64 SystemMonitor::CPUMonitor::GetTotalCpuPercentage() const {
	return can_read_cpu ? total_usage : 0;
}


double SystemMonitor::CPUMonitor::GetProcessCpuPercentage() const {
	return process_usage * 100.0;
}




//----------------------------------------------------------------------------------
//
// MemoryMonitor
//
//----------------------------------------------------------------------------------

void SystemMonitor::MemoryMonitor::Init(HANDLE handle) {
	process_handle = handle;
	mem_info.dwLength = sizeof(MEMORYSTATUSEX);
}

void SystemMonitor::MemoryMonitor::Tick() {

	// Total memory
	GlobalMemoryStatusEx(&mem_info);


	// Process memory
	GetProcessMemoryInfo(process_handle, reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
}


u64 SystemMonitor::MemoryMonitor::GetPhysicalMemSize() const {
	return mem_info.ullTotalPhys;
}


u64 SystemMonitor::MemoryMonitor::GetVirtualMemSize() const {
	return mem_info.ullTotalPageFile;
}


u64 SystemMonitor::MemoryMonitor::GetTotalUsedPhysicalMem() const {
	return mem_info.ullTotalPhys - mem_info.ullAvailPhys;
}


u64 SystemMonitor::MemoryMonitor::GetProcessUsedPhysicalMem() const {
	return pmc.WorkingSetSize;
}


u64 SystemMonitor::MemoryMonitor::GetTotalUsedVirtualMem() const {
	return mem_info.ullTotalPageFile - mem_info.ullAvailPageFile;
}


u64 SystemMonitor::MemoryMonitor::GetProcessUsedVirtualMem() const {
	return pmc.PrivateUsage;
}
