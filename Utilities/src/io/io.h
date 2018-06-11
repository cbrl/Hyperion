#pragma once

#include <fstream>
#include <filesystem>


// fstream
using std::fstream;
using std::ifstream;
using std::ofstream;

// wfstream
using std::wfstream;
using std::wifstream;
using std::wofstream;


// filesystem namespace
namespace fs = std::experimental::filesystem;


// Get the parent path of a file/folder
inline string GetParentPath(const string& in) {
	return fs::path(in).parent_path().string() + "/";
}

// Get the parent path of a file/folder
inline wstring GetParentPath(const wstring& in) {
	return fs::path(in).parent_path().wstring() + L"/";
}


// Get the name of a file
inline string GetFilename(const string& in) {
	return fs::path(in).filename().string();
}

// Get the name of a file
inline wstring GetFilename(const wstring& in) {
	return fs::path(in).filename().wstring();
}


// Get the extension of a file
inline string GetFileExtension(const string& in) {
	return fs::path(in).extension().string();
}

// Get the extension of a file
inline wstring GetFileExtension(const wstring& in) {
	return fs::path(in).extension().wstring();
}


// Get a file path through the Windows file picker
#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)

#include <Commdlg.h>
#include <Windows.h>

inline bool OpenFilePicker(wchar_t* lpstrFile, DWORD nMaxFile) {

	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = lpstrFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = nMaxFile;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return GetOpenFileName(&ofn) != 0;
}

#endif