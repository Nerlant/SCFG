#include "file_manager.h"

#include <string_view>
#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>

SCFG::FileManager::FileManager(const std::string_view path) : filePath(path)
{
	if (!std::filesystem::exists(filePath))
		throw std::runtime_error("FileManager::FileManager: Config file does not exist.");

	Load();
}

void SCFG::FileManager::OpenFile()
{
	if (!fileStream.is_open())
		fileStream.open(std::string(filePath), std::ios::in | std::ios::out | std::ios::binary);

	if (!fileStream.is_open())
		throw std::runtime_error("FileManager::OpenFile: Could not open config file.");
}

void SCFG::FileManager::CloseFile()
{
	if (fileStream.is_open())
		fileStream.close();
}

void SCFG::FileManager::Load()
{
	OpenFile();

	fileStream.seekg(0, std::ios::end);
	const auto size = static_cast<const size_t>(fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);

	file.resize(size);
	fileStream.read(file.data(), size);

	CloseFile();
}

void SCFG::FileManager::Safe()
{
	OpenFile();

	fileStream.seekp(0, std::ios::beg);

	file.shrink_to_fit(); // TODO: this is not needed - remove
	fileStream.write(file.data(), file.size());

	CloseFile();
}

std::vector<char> SCFG::FileManager::Read(const uintptr_t file_address, const size_t size)
{
	return std::vector<char>(file.begin() + file_address, file.begin() + file_address + size);
}

void SCFG::FileManager::Write(const uintptr_t file_address, const char* const data, const size_t size)
{
	// Resize file if data goes beyond current file boundaries
	if (file_address + size >= file.size())
		file.resize(file_address + size);

	std::copy(data, data + size, file.begin() + file_address);
}
