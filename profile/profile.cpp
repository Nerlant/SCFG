#include "profile.h"
#include "../scfg.h"
#include "../file_manager/file_manager.h"


SCFG::Profile::Profile(const std::string_view name, size_t file_offset, const FileManager& file_manager, const std::map<std::string, uint32_t>& type_map)
	: profileName(name), fileOffset(file_offset), fileManager(file_manager), typeMap(type_map)
{
}

std::vector<char> SCFG::Profile::GetData() const
{
	std::vector<char> data;
	for (const auto& [key, valueContainer] : valueMap)
	{
		const auto currentData = valueContainer->Save();
		data.insert(data.end(), 
			std::make_move_iterator(currentData.begin()), 
			std::make_move_iterator(currentData.end()));
	}

	return data;
}
