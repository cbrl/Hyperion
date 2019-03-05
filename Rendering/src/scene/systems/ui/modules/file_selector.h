#pragma once

#include "io/io.h"

class FileSelector final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	FileSelector(fs::path root_dir = fs::current_path());
	FileSelector(const FileSelector&) = default;
	FileSelector(FileSelector&&) = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~FileSelector() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	FileSelector& operator=(const FileSelector&) = default;
	FileSelector& operator=(FileSelector&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void openSelector();

	[[nodiscard]]
	bool update();

	[[nodiscard]]
	fs::path getSelectedFile();

private:

	void setPath(const fs::path& path);

	void listFiles(std::filesystem::directory_iterator it);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	bool open;
	std::string nav_bar_text;
	fs::path current_path;
	fs::path selected_file;
};