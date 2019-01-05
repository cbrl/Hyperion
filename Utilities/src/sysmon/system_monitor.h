#pragma once

#include "os/windows/windows.h"
#include "os/windows/win_utils.h"
#include <Psapi.h>
#include <thread> //std::thread::hardware_concurrency()

#include "datatypes/scalar_types.h"
#include "timer/timer.h"


struct PerCoreClock final {
	using rep        = u64;
	using period     = std::ratio<1, 10000000>;
	using duration   = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<PerCoreClock>;

	static constexpr bool is_steady = true;

	[[nodiscard]]
	static time_point now() noexcept {
		static const u16 core_count = std::thread::hardware_concurrency();
		return time_point(duration(GetCPUTime() / core_count));
	}
};

struct SystemWorkClock final {
	using rep        = u64;
	using period     = std::ratio<1, 10000000>;
	using duration   = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<SystemWorkClock>;

	static constexpr bool is_steady = true;

	[[nodiscard]]
	static time_point now() noexcept {
		static const u16 core_count = std::thread::hardware_concurrency();
		return time_point(duration(GetSystemWorkTime() / core_count));
	}
};

struct SystemIdleClock final {
	using rep        = u64;
	using period     = std::ratio<1, 10000000>;
	using duration   = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<SystemIdleClock>;

	static constexpr bool is_steady = true;

	[[nodiscard]]
	static time_point now() noexcept {
		static const u16 core_count = std::thread::hardware_concurrency();
		return time_point(duration(GetSystemIdleTime() / core_count));
	}
};

using SystemWorkTimer = Timer<SystemWorkClock>;
using SystemIdleTimer = Timer<SystemIdleClock>;
using PerCoreTimer = Timer<PerCoreClock>;


class SystemMonitor final {

	//----------------------------------------------------------------------------------
	// CPU Monitor
	//----------------------------------------------------------------------------------
	class CpuMonitor final {
		friend class SystemMonitor;

	protected:
		CpuMonitor() noexcept
			: sys_usage(0)
			, proc_usage(0) {
		}

		~CpuMonitor() = default;

		// Update the CPU stats
		void tick();


	public:
		[[nodiscard]]
		f64 getTotalCpuPercentage() const;

		[[nodiscard]]
		f64 getProcessCpuPercentage() const;


	private:
		f64 sys_usage;
		f64 proc_usage;

		SystemWorkTimer sys_work_timer;
		SystemIdleTimer sys_idle_timer;
		PerCoreTimer core_timer;
		HighResTimer wall_timer;
	};


	//----------------------------------------------------------------------------------
	// Memory Monitor
	//----------------------------------------------------------------------------------
	class MemoryMonitor final {
		friend class SystemMonitor;

	protected:
		MemoryMonitor() noexcept
			: mem_info{}
			, pmc{} {
			mem_info.dwLength = sizeof(MEMORYSTATUSEX);
		}

		~MemoryMonitor() = default;

		// Update the memory stats
		void tick();


	public:
		//----------------------------------------------------------------------------------
		// Physical Memory
		//----------------------------------------------------------------------------------
		[[nodiscard]]
		u64 getPhysicalMemSize() const;

		[[nodiscard]]
		u64 getTotalUsedPhysicalMem() const;

		[[nodiscard]]
		u64 getProcessUsedPhysicalMem() const;


		//----------------------------------------------------------------------------------
		// Virtual Memory
		//----------------------------------------------------------------------------------
		[[nodiscard]]
		u64 getVirtualMemSize() const;

		[[nodiscard]]
		u64 getTotalUsedVirtualMem() const;

		[[nodiscard]]
		u64 getProcessUsedVirtualMem() const;


	private:
		MEMORYSTATUSEX mem_info;
		PROCESS_MEMORY_COUNTERS_EX pmc;
	};


public:
	SystemMonitor() noexcept
		: dt(0.0) {
	}

	~SystemMonitor() = default;

	// Update resource stats
	void tick() {
		wall_timer.tick();
		dt += wall_timer.deltaTime();

		// Update every 500ms
		if (dt >= 0.5) {
			cpu_mon.tick();
			memory_mon.tick();
			dt = 0.0;
		}
	}

	// Access CPU stats
	const CpuMonitor& cpu() const {
		return cpu_mon;
	}

	// Access memory stats
	const MemoryMonitor& memory() const {
		return memory_mon;
	}


private:
	f64 dt;
	HighResTimer wall_timer;

	CpuMonitor cpu_mon;
	MemoryMonitor memory_mon;
};
