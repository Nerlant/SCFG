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
		void Save();

		template<class T>
		[[nodiscard]] T Read(const uintptr_t file_offset) const
		{
			return *reinterpret_cast<T*>(Read(file_offset, sizeof(T)).data());
		}

		[[nodiscard]] std::vector<char>Read(uintptr_t file_address, size_t size) const;

		template<class T>
		void Write(const uintptr_t file_offset, const T& value)
		{
			Write(file_offset, reinterpret_cast<const char*>(&value), sizeof(T));
		}

		void Write(uintptr_t file_address, const char* data, size_t size);

	private:
		std::string_view filePath;
		std::vector<char> file;

		std::fstream fileStream;
	};
}
