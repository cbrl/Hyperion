#pragma once

#pragma comment(lib, "pdh.lib")

#include <Windows.h>
#include <Pdh.h>
#include <Psapi.h>
#include "util\datatypes\datatypes.h"


class SystemMonitor {

	class CPUMonitor {
		friend class SystemMonitor;

		protected:
			CPUMonitor() = default;
			~CPUMonitor();

			void Init(HANDLE handle);
			void Tick();


		public:
			u32    GetTotalCpuPercentage() const;
			double GetProcessCpuPercentage() const;


		private:
			// Variables for total CPU usage
			bool     can_read_cpu;
			HQUERY   query_handle;
			HCOUNTER counter_handle;
			u64      last_sample_time;
			u64      total_usage;


			// Variables for process CPU usage
			HANDLE process_handle;
			u32    processor_count;
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


	class MemoryMonitor {
		friend class SystemMonitor;

		protected:
			MemoryMonitor() = default;
			~MemoryMonitor() = default;

			void Init(HANDLE handle);
			void Tick();


		public:
			u64  GetPhysicalMemSize() const;
			u64  GetVirtualMemSize() const;

			u64  GetTotalUsedPhysicalMem() const;
			u64  GetProcessUsedPhysicalMem() const;

			u64  GetTotalUsedVirtualMem() const;
			u64  GetProcessUsedVirtualMem() const;


		private:
			HANDLE process_handle;

			MEMORYSTATUSEX mem_info;
			PROCESS_MEMORY_COUNTERS_EX pmc;
	};


	public:
		SystemMonitor() {
			HANDLE handle = GetCurrentProcess();

			cpu_mon.Init(handle);
			memory_mon.Init(handle);
		}
		~SystemMonitor() = default;

		// Update resource stats
		void Tick() {
			cpu_mon.Tick();
			memory_mon.Tick();
		}

		// Access CPU stats
		const CPUMonitor&    CPU()    { return cpu_mon; }

		// Access memory stats
		const MemoryMonitor& Memory() { return memory_mon; }


	private:
		CPUMonitor    cpu_mon;
		MemoryMonitor memory_mon;
};
