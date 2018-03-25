#pragma once

#include <fstream>
#include <sstream>
#include <filesystem>


// fstream
using std::fstream;
using std::ifstream;
using std::ofstream;

// wfstream
using std::wfstream;
using std::wifstream;
using std::wofstream;

// stringstream
using std::stringstream;
using std::istringstream;
using std::ostringstream;

// wstringstream
using std::wstringstream;
using std::wistringstream;
using std::wostringstream;

// filesystem namespace
namespace fs = std::experimental::filesystem;



// Get the extension of a file
inline string GetFileExtension(const string& in) {
	return fs::path(in).extension().string();
}

// Get the extension of a file
inline wstring GetFileExtension(const wstring& in) {
	return fs::path(in).extension().wstring();
}