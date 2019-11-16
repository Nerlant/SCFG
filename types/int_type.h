#pragma once

#include "type.h"

#include <vector>


namespace SCFG::Type
{
	template<> std::vector<char> SaveType(const int& value);

	template<> int LoadType(const char* data);
}
