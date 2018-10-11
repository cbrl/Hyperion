#pragma once

#include <fstream>
#include <filesystem>


// filesystem namespace alias
namespace fs = std::filesystem;


// Get the parent path of a file/folder
inline std::string GetParentPath(const std::string& in) {
	return fs::path(in).parent_path().string() + "/";
}

// Get the parent path of a file/folder
inline std::wstring GetParentPath(const std::wstring& in) {
	return fs::path(in).parent_path().wstring() + L"/";
}


// Get the name of a file
inline std::string GetFilename(const std::string& in) {
	return fs::path(in).filename().string();
}

// Get the name of a file
inline std::wstring GetFilename(const std::wstring& in) {
	return fs::path(in).filename().wstring();
}


// Get the extension of a file
inline std::string GetFileExtension(const std::string& in) {
	return fs::path(in).extension().string();
}

// Get the extension of a file
inline std::wstring GetFileExtension(const std::wstring& in) {
	return fs::path(in).extension().wstring();
}
