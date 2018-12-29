#pragma once


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include "os/windows/windows.h"
#include "io/io.h"

#include <Commdlg.h>
#include <ShObjIdl_core.h>




//----------------------------------------------------------------------------------
// Functions - System Time
//----------------------------------------------------------------------------------

// Convert a time represetned by a FILETIME struct
// to a single 64 bit integer (units: 100ns)
inline u64 FiletimeToU64(const FILETIME& time) {
	return static_cast<u64>(time.dwLowDateTime)
	       | (static_cast<u64>(time.dwHighDateTime) << 32);
}


// Outputs the amount of time the process has
// executed in kernel and user modes (units: 100ns)
inline void GetProcessTimes(u64& kernel_time, u64& user_time) {
	FILETIME ftime;
	FILETIME fkernel;
	FILETIME fuser;

	const BOOL result = GetProcessTimes(GetCurrentProcess(), &ftime, &ftime, &fkernel, &fuser);

	if (result == TRUE) {
		kernel_time = FiletimeToU64(fkernel);
		user_time   = FiletimeToU64(fuser);
	}
	else {
		kernel_time = 0;
		user_time   = 0;
	}
}


// Outputs the amount of time the system has
// executed in kernel and user modes (units: 100ns)
inline void GetSystemWorkTimes(u64& kernel_time, u64& user_time) {
	FILETIME fidle;
	FILETIME fkernel;
	FILETIME fuser;

	const BOOL result = GetSystemTimes(&fidle, &fkernel, &fuser);

	if (result == TRUE) {
		kernel_time = FiletimeToU64(fkernel);
		user_time   = FiletimeToU64(fuser);
	}
	else {
		kernel_time = 0;
		user_time   = 0;
	}
}


// Returns the total time the system has been idle
inline u64 GetSystemIdleTime() {
	FILETIME fidle;
	FILETIME fkernel;
	FILETIME fuser;

	const BOOL result = GetSystemTimes(&fidle, &fkernel, &fuser);

	return result == TRUE ? FiletimeToU64(fidle) : 0;
}


// Returns the total time the system has executed (kernel + user, units: 100ns)
inline u64 GetSystemWorkTime() {
	u64 kernel_time = 0;
	u64 user_time = 0;
	GetSystemWorkTimes(kernel_time, user_time);
	return kernel_time + user_time;
}


// Returns the total time the process has executed (kernel + user, units: 100ns)
inline u64 GetCPUTime() {
	u64 kernel_time = 0;
	u64 user_time = 0;
	GetProcessTimes(kernel_time, user_time);
	return kernel_time + user_time;
}




//----------------------------------------------------------------------------------
// Functions - I/O
//----------------------------------------------------------------------------------

// Get a file path through the Windows file picker
inline fs::path OpenFilePicker() {
	ComPtr<IFileOpenDialog> pFileOpen;

	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
	                              NULL,
	                              CLSCTX_ALL,
	                              IID_IFileOpenDialog,
	                              reinterpret_cast<void**>(pFileOpen.GetAddressOf()));

	if (SUCCEEDED(hr)) {
		// Show the file dialog
		hr = pFileOpen->Show(NULL);

		if (SUCCEEDED(hr)) {
			// Get the file
			ComPtr<IShellItem> pItem;
			hr = pFileOpen->GetResult(pItem.GetAddressOf());

			if (SUCCEEDED(hr)) {
				// Store the file name
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				if (SUCCEEDED(hr)) {
					CoTaskMemFree(pszFilePath);
					return fs::path{pszFilePath};
				}
			}
		}
	}

	return fs::path{};
}


// Create a console window that redirects stdin, stdout, and stderr
inline void AllocateConsole() {

	ThrowIfFailed(AllocConsole() != 0, "Failed to create console");

	// Direct stdin to the console
	{
		FILE* stream;
		const auto result = freopen_s(&stream, "CONIN$", "r", stdin);
		ThrowIfFailed(result == 0, "Failed to redirect stdin");
	}

	// Direct stdout to the console
	{
		FILE* stream;
		const auto result = freopen_s(&stream, "CONOUT$", "w", stdout);
		ThrowIfFailed(result == 0, "Failed to redirect stdout");
	}

	// Direct stderr to the console
	{
		FILE* stream;
		const auto result = freopen_s(&stream, "CONOUT$", "w", stderr);
		ThrowIfFailed(result == 0, "Failed to redirect stderr");
	}
	
}