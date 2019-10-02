#pragma once

#include <stdint.h>

struct Header
{
	uint8_t CharacteristicBytes[2];
	uint32_t Checksum;
	uint16_t HeaderSize;
	uint16_t NumberOfTypes;
	ptrdiff_t FirstTypeEntry; // Type entries are stored behind each other in memory, offset from file start to first type entry
};
