#pragma once

#include <vector>
#include <fstream>

namespace SCFG
{
	class FileManager
	{
	public:
		FileManager() = delete;
		explicit FileManager(std::string_view path);

		~FileManager() = default;

		void OpenFile();
		void CloseFile();

		void Load();
		void Safe();

		std::vector<char>Read(uintptr_t file_address, size_t size);
		void Write(uintptr_t file_address, const char* data, size_t size);

	private:
		std::string_view filePath;
		std::vector<char> file;

		std::fstream fileStream;
	};
}
