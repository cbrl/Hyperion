module;

#include "datatypes/scalar_types.h"
#include "time/stopwatch.h"
#include "os/windows/windows.h"

#include <Psapi.h>
#include <thread> //std::thread::hardware_concurrency()

export module system_monitor;

import win_utils;

export {

struct PerCoreClock final {
	using rep        = u64;
	using period     = std::ratio<1, 10'000'000>;
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
	using period     = std::ratio<1, 10'000'000>;
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
	using period     = std::ratio<1, 10'000'000>;
	using duration   = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<SystemIdleClock>;

	static constexpr bool is_steady = true;

	[[nodiscard]]
	static time_point now() noexcept {
		static const u16 core_count = std::thread::hardware_concurrency();
		return time_point(duration(GetSystemIdleTime() / core_count));
	}
};

using SystemWorkTimer = Stopwatch<SystemWorkClock>;
using SystemIdleTimer = Stopwatch<SystemIdleClock>;
using PerCoreTimer = Stopwatch<PerCoreClock>;


class SystemMonitor final {

	//----------------------------------------------------------------------------------
	// CPU Monitor
	//----------------------------------------------------------------------------------
	class CpuMonitor final {
		friend class SystemMonitor;

	protected:
		//----------------------------------------------------------------------------------
		// Constructors
		//----------------------------------------------------------------------------------
		CpuMonitor() = default;
		CpuMonitor(const CpuMonitor&) = default;
		CpuMonitor(CpuMonitor&&) = default;

		//----------------------------------------------------------------------------------
		// Destructor
		//----------------------------------------------------------------------------------
		~CpuMonitor() = default;

		//----------------------------------------------------------------------------------
		// Operators
		//----------------------------------------------------------------------------------
		CpuMonitor& operator=(const CpuMonitor&) = default;
		CpuMonitor& operator=(CpuMonitor&&) = default;

		//----------------------------------------------------------------------------------
		// Member Functions
		//----------------------------------------------------------------------------------

		// Update the CPU stats
		void tick() {
			// Update wall clock
			wall_timer.tick();

			// Update system clocks
			core_timer.tick();
			sys_work_timer.tick();
			sys_idle_timer.tick();

			// Update cpu stats
			sys_usage  = ((sys_work_timer.deltaTime() - sys_idle_timer.deltaTime()) * 100.0) / sys_work_timer.deltaTime();
			proc_usage = (core_timer.deltaTime() * 100.0) / wall_timer.deltaTime();
		}

	public:

		[[nodiscard]]
		f64 getTotalCpuPercentage() const noexcept {
			return sys_usage;
		}

		[[nodiscard]]
		f64 getProcessCpuPercentage() const noexcept {
			return proc_usage;
		}

	private:

		//----------------------------------------------------------------------------------
		// Member Variables
		//----------------------------------------------------------------------------------
		f64 sys_usage = 0;
		f64 proc_usage = 0;

		SystemWorkTimer sys_work_timer;
		SystemIdleTimer sys_idle_timer;
		PerCoreTimer core_timer;
		Stopwatch<> wall_timer;
	};



	//----------------------------------------------------------------------------------
	// Memory Monitor
	//----------------------------------------------------------------------------------
	class MemoryMonitor final {
		friend class SystemMonitor;

	protected:
		//----------------------------------------------------------------------------------
		// Constructors
		//----------------------------------------------------------------------------------
		MemoryMonitor() noexcept
			: mem_info{}
			, pmc{} {
			mem_info.dwLength = sizeof(MEMORYSTATUSEX);
			tick();
		}

		MemoryMonitor(const MemoryMonitor&) = default;
		MemoryMonitor(MemoryMonitor&&) = default;

		//----------------------------------------------------------------------------------
		// Destructor
		//----------------------------------------------------------------------------------
		~MemoryMonitor() = default;

		//----------------------------------------------------------------------------------
		// Operators
		//----------------------------------------------------------------------------------
		MemoryMonitor& operator=(const MemoryMonitor&) = default;
		MemoryMonitor& operator=(MemoryMonitor&&) = default;

		//----------------------------------------------------------------------------------
		// Member Functions - Tick
		//----------------------------------------------------------------------------------

		// Update the memory stats
		void tick() {
			GlobalMemoryStatusEx(&mem_info); //Total memory
			GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc)); //Process memory
		}

	public:

		//----------------------------------------------------------------------------------
		// Member Functions - Physical Memory
		//----------------------------------------------------------------------------------
		[[nodiscard]]
		u64 getPhysicalMemSize() const noexcept {
			return mem_info.ullTotalPhys;
		}

		[[nodiscard]]
		u64 getTotalUsedPhysicalMem() const noexcept {
			return mem_info.ullTotalPhys - mem_info.ullAvailPhys;
		}

		[[nodiscard]]
		u64 getProcessUsedPhysicalMem() const noexcept {
			return pmc.WorkingSetSize;
		}


		//----------------------------------------------------------------------------------
		// Member Functions - Virtual Memory
		//----------------------------------------------------------------------------------
		[[nodiscard]]
		u64 getVirtualMemSize() const noexcept {
			return mem_info.ullTotalPageFile;
		}

		[[nodiscard]]
		u64 getTotalUsedVirtualMem() const noexcept {
			return mem_info.ullTotalPageFile - mem_info.ullAvailPageFile;
		}

		[[nodiscard]]
		u64 getProcessUsedVirtualMem() const noexcept {
			return pmc.PrivateUsage;
		}

	private:

		//----------------------------------------------------------------------------------
		// Member Variables
		//----------------------------------------------------------------------------------
		MEMORYSTATUSEX mem_info;
		PROCESS_MEMORY_COUNTERS_EX pmc;
	};



public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SystemMonitor() = default;
	SystemMonitor(const SystemMonitor&) = default;
	SystemMonitor(SystemMonitor&&) = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~SystemMonitor() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SystemMonitor& operator=(const SystemMonitor&) = default;
	SystemMonitor& operator=(SystemMonitor&&) = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Update resource stats
	void tick() {
		wall_timer.tick();
		if (wall_timer.totalTime() >= update_interval) {
			cpu_mon.tick();
			memory_mon.tick();
			wall_timer.reset();
		}
	}

	// Get the time between updates
	[[nodiscard]]
	std::chrono::milliseconds getUpdateInterval() const noexcept {
		return update_interval;
	}

	// Set the time between updates
	void setUpdateInterval(std::chrono::milliseconds time) noexcept {
		update_interval = time;
	}

	// Access CPU stats
	const CpuMonitor& cpu() const noexcept {
		return cpu_mon;
	}

	// Access memory stats
	const MemoryMonitor& memory() const noexcept {
		return memory_mon;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	Stopwatch<> wall_timer;
	std::chrono::milliseconds update_interval = 200ms;

	CpuMonitor cpu_mon;
	MemoryMonitor memory_mon;
};

} //export
