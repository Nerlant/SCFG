#include "profile.h"
#include "../scfg.h"


SCFG::Profile::Profile(const std::string_view name, SCFG& scfg) : profileName(name), scfg(scfg)
{}

std::vector<char> SCFG::Profile::WriteProfile()
{
	std::vector<char> data(profileName.length());
	std::copy(profileName.begin(), profileName.end(), data.begin());

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
