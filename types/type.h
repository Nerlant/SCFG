#pragma once

#include "../not_implemented_exception.h"


namespace SCFG::Type
{
	// template T with not implemented exception, so for each supported type they need to be specified
	template<class T>
	void SafeType(const T& value, char* file_location)
	{
		throw Exception::NotImplementedException();
	}

	template<class T>
	T LoadType(char* data)
	{
		throw Exception::NotImplementedException();
	}
	
}
