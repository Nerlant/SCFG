#pragma once

#include <cstdint>


namespace SCFG
{
	/*
	 * The Header structs main purpose is to illustrate the layout of the meta information of the config file
	 *
	 * CharacteristicBytes: First 4 bytes to check if provided file is a valid scfg file. They must contain "SCFG".
	 * NumberOfFields: The number of fields stored in each profile.
	 * Options: Bitfield to store additional options. Unused.
	 */
	
	struct Header
	{
		uint32_t CharacteristicBytes;
		uint16_t NumberOfFields;
		uint16_t Options;
	};
}
