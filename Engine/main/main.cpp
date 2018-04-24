#include "stdafx.h"
#include "util\datatypes\datatypes.h"
#include "util\engine_util.h"
#include "system\System.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Set the logging level
	//FILELog::ReportingLevel() = FILELog::FromString(argv[1] ? argv[1] : "DEBUG1");
	FILELog::ReportingLevel() = logDEBUG1;


	// Open the log file and set it as the log destination
	FILE* log;
	fopen_s(&log, "log.txt", "a");
	Output2FILE::Stream() = log;

	FILE_LOG(logINFO) << "<=========================START=========================>";


	// Enable run-time memory check for debug builds
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif


	// Create system object
	unique_ptr<System> system = make_unique<System>();


	// Initialize system
	if (!system->Init()) {
		return 1;
	}

	// Run the system
	FILE_LOG(logINFO) << "Running...";
	system->Run();


	// Quit the system
	FILE_LOG(logINFO) << "Quitting...";
	system.reset();


	// Close the log file
	FILE_LOG(logINFO) << "<==========================END==========================>" << "\n";
	fclose(log);


	return 0;
}