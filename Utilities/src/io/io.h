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
namespace fs = std::filesystem;


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
