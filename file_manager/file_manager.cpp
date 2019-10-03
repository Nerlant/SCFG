#include "file_manager.h"

#include <string_view>
#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>

FileManager::FileManager(const std::string_view path) : filePath(path)
{
	if (!std::filesystem::exists(filePath))
		throw std::runtime_error("FileManager::FileManager: Config file does not exist.");

	Load();
}

void FileManager::OpenFile()
{
	if (!fileStream.is_open())
		fileStream.open(std::string(filePath), std::ios::in | std::ios::out | std::ios::binary);

	if (!fileStream.is_open())
		throw std::runtime_error("FileManager::OpenFile: Could not open config file.");
}

void FileManager::CloseFile()
{
	if (fileStream.is_open())
		fileStream.close();
}

void FileManager::Load()
{
	OpenFile();

	fileStream.seekg(0, std::ios::end);
	const auto size = static_cast<const size_t>(fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);

	file.resize(size);
	fileStream.read(file.data(), size);

	CloseFile();
}

void FileManager::Safe()
{
	OpenFile();

	fileStream.seekp(0, std::ios::beg);

	file.shrink_to_fit(); // TODO: this is not needed - remove
	fileStream.write(file.data(), file.size());

	CloseFile();
}

std::vector<char> FileManager::Read(const uintptr_t file_address, const size_t size)
{
	return std::vector<char>(file.begin() + file_address, file.begin() + file_address + size);
}

void FileManager::Write(const uintptr_t file_address, const char* const data, const size_t size)
{
	// Resize file if data goes beyond current file boundaries
	if (file_address + size >= file.size())
		file.resize(file_address + size);

	std::copy(data, data + size, file.begin() + file_address);
}
