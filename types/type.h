#pragma once

#include "../not_implemented_exception.h"

#include <vector>


namespace SCFG::Type
{
	template<class T>
	std::vector<char> SafeType(const T& value)
	{
		throw Exception::NotImplementedException();
	}

	template<class T>
	T LoadType(const char* data)
	{
		throw Exception::NotImplementedException();
	}
	
}
