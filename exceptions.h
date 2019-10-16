#pragma once

#include <stdexcept>
#include <string>


namespace SCFG::Exception
{
	class NotImplementedException final : public std::logic_error
	{
	public:
		NotImplementedException() : std::logic_error{ "Function not yet implemented." } {}
	};

	class InvalidConfigFileException final : public std::runtime_error
	{
	public:
		InvalidConfigFileException() : std::runtime_error("Wrong image magic of config file.") {}
	};

	class TypeMismatchException final : public std::runtime_error
	{
	public:
		TypeMismatchException() : std::runtime_error("Mismatch of type size between type map and file: type index: ") {}
	};

	class InvalidTypeIndexException final : public std::runtime_error
	{
	public:
		InvalidTypeIndexException(const std::string& exception_msg) : std::runtime_error("Type index is out of range: " + exception_msg) {}
	};
	
}
