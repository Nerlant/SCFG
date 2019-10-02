#pragma once

#include <string_view>
#include <stdint.h>
#include <vector>
#include <fstream>

class FileManager
{
public:
	FileManager(std::string_view path);
	FileManager() = delete;

	~FileManager();

	void OpenFile();
	void CloseFile();

	void Load();
	void Safe();

	std::vector<char>Read(uintptr_t file_address, size_t size);
	void Write(uintptr_t file_address, const char* const data, size_t size);

private:
	std::string_view filePath;
	std::vector<char> file;

	std::fstream fileStream;
};
