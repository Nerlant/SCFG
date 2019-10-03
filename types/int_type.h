#pragma once

#include "type.h"


namespace SCFG::Type
{
	template<> void SafeType(const int& value, char* file_location);

	template<> int LoadType(char* data);
	
}
