#pragma once

#include "../exceptions.h"

#include <vector>
#include <functional>


namespace SCFG::Type
{
	template <class T>
	using SafeType_t = std::function<std::vector<char>(const T&)>;

	template <class T>
	using LoadType_t = std::function<T(const char*)>;
	
	template<class T>
	std::vector<char> SaveType(const T& value)
	{
		throw Exception::NotImplementedException();
	}

	template<class T>
	T LoadType(const char* data)
	{
		throw Exception::NotImplementedException();
	}
}
