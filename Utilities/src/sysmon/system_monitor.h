#pragma once

#include "os/windows.h"
#include <Psapi.h>
#include "datatypes/datatypes.h"
#include "timer/timer.h"


struct PerCoreClock {
	using rep        = u64;
	using period     = std::ratio<1, 10000000>;
	using duration   = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<PerCoreClock>;

	static constexpr bool is_steady = true;

	[[nodiscard]]
	static time_point now() noexcept {
		static u16 core_count = std::thread::hardware_concurrency();
		return time_point(duration(GetCPUTime() / core_count));
	}
};

struct SystemWorkClock {
	using rep = u64;
	using period = std::ratio<1, 10000000>;
	using duration = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<SystemWorkClock>;

	static constexpr bool is_steady = true;

	[[nodiscard]]
	static time_point now() noexcept {
		static u16 core_count = std::thread::hardware_concurrency();
		return time_point(duration(GetSystemWorkTime() / core_count));
	}
};

struct SystemIdleClock {
	using rep = u64;
	using period = std::ratio<1, 10000000>;
	using duration = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<SystemIdleClock>;

	static constexpr bool is_steady = true;

	[[nodiscard]]
	static time_point now() noexcept {
		static u16 core_count = std::thread::hardware_concurrency();
		return time_point(duration(GetSystemIdleTime() / core_count));
	}
};

using SystemWorkTimer = Timer<SystemWorkClock>;
using SystemIdleTimer = Timer<SystemIdleClock>;
using PerCoreTimer = Timer<PerCoreClock>;


class SystemMonitor final {

	class CpuMonitor final {
		friend class SystemMonitor;

	protected:
		CpuMonitor()
			: dt(0)
			, sys_usage(0)
			, proc_usage(0) {
		}

		~CpuMonitor() = default;

		// Update the CPU stats
		void tick();


	public:
		double getTotalCpuPercentage() const;
		double getProcessCpuPercentage() const;


	private:
		double dt;
		double sys_usage;
		double proc_usage;

		SystemWorkTimer sys_work_timer;
		SystemIdleTimer sys_idle_timer;
		PerCoreTimer core_timer;
		HighResTimer wall_timer;
	};


	class MemoryMonitor final {
		friend class SystemMonitor;

	protected:
		MemoryMonitor() {
			mem_info.dwLength = sizeof(MEMORYSTATUSEX);
		}

		~MemoryMonitor() = default;

		// Update the memory stats
		void tick();


	public:
		u64 getPhysicalMemSize() const;
		u64 getVirtualMemSize() const;

		u64 getTotalUsedPhysicalMem() const;
		u64 getProcessUsedPhysicalMem() const;

		u64 getTotalUsedVirtualMem() const;
		u64 getProcessUsedVirtualMem() const;


	private:
		MEMORYSTATUSEX mem_info;
		PROCESS_MEMORY_COUNTERS_EX pmc;
	};


public:
	SystemMonitor() = default;
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
