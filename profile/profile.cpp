#include "profile.h"
#include "../scfg.h"


SCFG::Profile::Profile(const std::string_view name, const size_t file_offset, SCFG& scfg) : profileName(name), fileOffset(file_offset), scfg(scfg)
{}

std::vector<char> SCFG::Profile::WriteProfile() const
{
	std::vector<char> data;
	for (const auto& [key, typeContainer] : valueMap)
	{
		const auto currentData = typeContainer->Save();
		data.insert(data.end(), 
			std::make_move_iterator(currentData.begin()), 
			std::make_move_iterator(currentData.end()));
	}

	return data;
}

bool SCFG::Profile::getFieldFromCfg(const std::string_view name)
{
	// TODO: implement
	throw Exception::NotImplementedException();
}
