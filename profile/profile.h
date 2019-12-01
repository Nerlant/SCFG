#pragma once

#include "../types/value_container.h"
#include "../file_manager/file_manager.h"
#include "../exceptions.h"

#include <vector>
#include <memory>
#include <string_view>
#include <string>
#include <map>


namespace SCFG
{
	struct FieldInfo
	{
		explicit FieldInfo(const uint32_t size, const uint32_t file_offset, const uint32_t file_size) noexcept :
			Size(size), FileOffset(file_offset), FileSize(file_size)
		{}
		
		uint32_t Size;
		uint32_t FileOffset;
		uint32_t FileSize;
	};
	
	class Profile
	{
		// Needs to contain all the settings
	public:
		explicit Profile(size_t file_offset, FileManager& file_manager, const std::map<std::string, FieldInfo, std::less<>>& type_map);

		template <class T>
		T GetValueByName(const std::string_view name)
		{
			// Try to get cached field
			if (const auto pair = valueMap.find(name); pair != valueMap.end())
				return dynamic_cast<ValueContainer<T>*>(pair->second.get())->GetValue();

			// Read field from config
			if (!getFieldFromCfg<T>(name))
				throw Exception::InvalidFieldNameException(std::string(name));
			
			// Try again to get value
			return GetValueByName<T>(name);
		}

		template <class T>
		bool SetValue(const std::string_view name, const T& value)
		{
			return valueMap.insert_or_assign(std::string(name), std::make_shared<ValueContainer<T>>(value)).second;
		}

		[[nodiscard]] size_t Save(size_t offset) const;

	private:
		std::map <std::string, std::shared_ptr<ValueContainerBase>, std::less<>> valueMap;
		size_t fileOffset;
		FileManager& fileManager;
		const std::map<std::string, FieldInfo, std::less<>>& typeMap;

		template <class T>
		bool getFieldFromCfg(const std::string_view name)
		{
			if (const auto it = typeMap.find(name); it != typeMap.end())
			{
				const auto fieldData = fileManager.Read(it->second.FileOffset, it->second.FileSize);
				T value;
				std::memcpy(&value, fieldData.data(), it->second.FileSize);

				return SetValue(name, value);
			}

			throw Exception::InvalidFieldNameException(std::string(name));
		}
	};
}
