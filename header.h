#pragma once

#include <cstdint>


namespace SCFG
{
	/*
	 * The Header structs main purpose is to illustrate the layout of the meta information of the config file
	 *
	 * CharacteristicBytes: First 4 bytes to check if provided file is a valid scfg file. They must contain "SCFG".
	 * NumberOfTypes: The number of file entries stored in the config file
	 * FirstTypeEntry: The first type entry. A type entry contains of the Type::List index and the size of the type.
	 */
	
	struct Header
	{
		uint32_t CharacteristicBytes;
		uint16_t NumberOfTypes;
		uint32_t FirstTypeEntry; // Type entries are stored behind each other in memory, offset from file start to first type entry
	};
}
