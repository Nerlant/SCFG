#include "int_type.h"

#include <vector>


template <>
std::vector<char> SCFG::Type::SaveType<int>(const int& value)
{
	std::vector<char> data(4);
	std::memcpy(data.data(), &value, 4);
	return data;
}

template <>
int SCFG::Type::LoadType<int>(const char* data) noexcept
{
	return *reinterpret_cast<const int*>(data);
}

template<>
uint32_t SCFG::Type::GetTypeSize<int>() noexcept
{
	return 4;
}
