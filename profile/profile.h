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
	class Profile
	{
		// Needs to contain all the settings
	public:
		explicit Profile(size_t file_offset, const FileManager& file_manager, const std::map<std::string, uint32_t, std::less<>>& type_map);

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
			return valueMap.emplace(name, std::make_shared<ValueContainer<T>>(value)).second;
		}

		[[nodiscard]] std::vector<char> GetData() const;

	private:
		std::map <std::string, std::shared_ptr<ValueContainerBase>, std::less<>> valueMap;
		size_t fileOffset;
		const FileManager& fileManager;
		const std::map<std::string, uint32_t, std::less<>>& typeMap;

		template <class T>
		bool getFieldFromCfg(const std::string_view name)
		{
			auto fieldFileOffset = fileOffset;
			size_t fieldFileSize = 0;
			for (const auto& [curName, size] : typeMap)
			{
				if (curName == name)
				{
					fieldFileSize = size;
					break;
				}

				fieldFileOffset += size;
			}

			if (!fieldFileSize)
				throw Exception::InvalidFieldNameException(std::string(name));

			const auto fieldData = fileManager.Read(fieldFileOffset, fieldFileSize);
			T value;
			std::memcpy(&value, fieldData.data(), fieldFileSize);

			return SetValue(name, value);
		}
	};
}
