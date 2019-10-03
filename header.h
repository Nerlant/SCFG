#pragma once

#include <cstdint>

struct Header
{
	uint8_t CharacteristicBytes[2];
	uint16_t HeaderSize;
	uint16_t NumberOfTypes;
	ptrdiff_t FirstTypeEntry; // Type entries are stored behind each other in memory, offset from file start to first type entry
};
