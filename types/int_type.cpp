#include "int_type.h"

template <>
void SCFG::Type::SafeType<int>(const int& value, char* file_location)
{
	*reinterpret_cast<int*>(file_location) = value;
}

template <>
int SCFG::Type::LoadType<int>(char* data)
{
	return *reinterpret_cast<int*>(data);
}
