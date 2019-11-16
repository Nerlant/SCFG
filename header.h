#pragma once

#include <cstdint>


namespace SCFG
{
	/*
	 * The Header structs main purpose is to illustrate the layout of the meta information of the config file
	 *
	 * CharacteristicBytes: First 4 bytes to check if provided file is a valid scfg file. They must contain "SCFG".
	 * NumberOfProfiles: The number of profiles stored inside the cfg.
	 * NumberOfFields: The number of fields stored in each profile.
	 */
	
	struct Header
	{
		uint32_t CharacteristicBytes;
		uint16_t NumberOfProfiles;
		uint16_t NumberOfFields;
	};
}
