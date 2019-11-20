#pragma once

#include "../types/value_container.h"
#include "../file_manager/file_manager.h"
#include "../exceptions.h"

#include <vector>
#include <memory>
#include <string_view>
#include <map>

// TODO: implement

// each profile must contain:
// map from name to index - maybe not needed but we need to safe the order how the values are saved somewhere (do we?)
// map from index to typeContainer
//
// maybe map from name to typeContainer is enough

namespace SCFG
{
	class Profile
	{
		// Needs to contain all the settings
	public:
		explicit Profile(std::string_view name, size_t file_offset, const FileManager& file_manager, const std::map<std::string, uint32_t>& type_map); // TODO: can this be a const SCFG& ?

		template <class T>
		T GetValueByName(const std::string_view name)
		{
			try
			{
				return dynamic_cast<ValueContainer<T>*>(valueMap.at(name).get())->GetValue();
			}
			catch (const std::out_of_range& ex)
			{
				if (!getFieldFromCfg<T>(name))
					throw Exception::InvalidFieldNameException(ex.what());
				
				/* Get Value from cfg and cache it
				*
				* for that we need: the name list, access to the file manager
				*/

				// Try again to get value
				return GetValueByName<T>(name);
			}
		}

		template <class T>
		bool SetValue(const std::string_view name, const T& value)
		{
			return valueMap.emplace(name, std::make_shared<ValueContainer<T>>(value)).second;
		}

		[[nodiscard]] std::vector<char> GetData() const;

	private:
		std::string profileName;
		std::map<std::string_view, std::shared_ptr<ValueContainerBase>> valueMap;
		size_t fileOffset;
		const FileManager& fileManager;
		const std::map<std::string, uint32_t>& typeMap;

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
			std::copy(fieldData.begin(), fieldData.end(), &value);

			return SetValue(name, value);
		}
	};
}
