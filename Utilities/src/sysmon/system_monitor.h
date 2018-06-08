#pragma once

#pragma comment(lib, "pdh.lib")

#include <Pdh.h>
#include <Psapi.h>
#include "datatypes/datatypes.h"


class SystemMonitor final {

	class CpuMonitor final {
		friend class SystemMonitor;

	protected:
		CpuMonitor() = default;
		~CpuMonitor();

		void init(HANDLE handle);
		void tick();


	public:
		u64 getTotalCpuPercentage() const;
		double getProcessCpuPercentage() const;


	private:
		// Variables for total CPU usage
		bool can_read_cpu;
		HQUERY query_handle;
		HCOUNTER counter_handle;
		u64 last_sample_time;
		u64 total_usage;


		// Variables for process CPU usage
		HANDLE process_handle;
		u32 processor_count;
		double process_usage;

		FILETIME ftime;
		FILETIME fsys;
		FILETIME fuser;

		ULARGE_INTEGER now;
		ULARGE_INTEGER sys;
		ULARGE_INTEGER user;

		ULARGE_INTEGER last_cpu;
		ULARGE_INTEGER last_sys_cpu;
		ULARGE_INTEGER last_user_cpu;
	};


	class MemoryMonitor final {
		friend class SystemMonitor;

	protected:
		MemoryMonitor() = default;
		~MemoryMonitor() = default;

		void init(HANDLE handle);
		void tick();


	public:
		u64 getPhysicalMemSize() const;
		u64 getVirtualMemSize() const;

		u64 getTotalUsedPhysicalMem() const;
		u64 getProcessUsedPhysicalMem() const;

		u64 getTotalUsedVirtualMem() const;
		u64 getProcessUsedVirtualMem() const;


	private:
		HANDLE process_handle;

		MEMORYSTATUSEX mem_info;
		PROCESS_MEMORY_COUNTERS_EX pmc;
	};


public:
	SystemMonitor() {
		const auto handle = GetCurrentProcess();

		cpu_mon.init(handle);
		memory_mon.init(handle);
	}

	~SystemMonitor() = default;

	// Update resource stats
	void tick() {
		cpu_mon.tick();
		memory_mon.tick();
	}

	// Access CPU stats
	const CpuMonitor& cpu() const { return cpu_mon; }

	// Access memory stats
	const MemoryMonitor& memory() const { return memory_mon; }


private:
	CpuMonitor cpu_mon;
	MemoryMonitor memory_mon;
};
