#include "profile.h"
#include "../scfg.h"
#include "../file_manager/file_manager.h"


SCFG::Profile::Profile(const size_t file_offset, FileManager& file_manager, const std::map<std::string, FieldInfo, std::less<>>& type_map)
	: fileOffset(file_offset), fileManager(file_manager), typeMap(type_map)
{
}

size_t SCFG::Profile::Save(const size_t offset) const
{
	for (const auto& [name, value] : valueMap)
	{
		if (const auto typeEntry = typeMap.find(name); typeEntry != typeMap.end())
		{
			fileManager.Write(offset + typeEntry->second.FileOffset, value->Save().data(), typeEntry->second.FileSize);
		}
		// TODO: handle 'else' branch, throw exception?
	}

	const auto lastTypeInfo = typeMap.rbegin()->second;
	return offset + lastTypeInfo.FileOffset + lastTypeInfo.FileSize;
}
