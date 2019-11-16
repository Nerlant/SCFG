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

	class SizeMismatchException final : public std::runtime_error
	{
	public:
		SizeMismatchException() : std::runtime_error("Mismatch of type size between type map and file.") {}
	};

	class InvalidFieldNameException final : public std::runtime_error
	{
	public:
		explicit InvalidFieldNameException(const std::string& exception_msg) : std::runtime_error("Type index is out of range: " + exception_msg) {}
	};

	class DuplicateFieldNameException final : public std::runtime_error
	{
	public:
		explicit DuplicateFieldNameException(const std::string& duplicate_name) : std::runtime_error("Duplicate field name: " + duplicate_name) {}
	};
	
}