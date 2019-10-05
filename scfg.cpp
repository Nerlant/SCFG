#include "scfg.h"

SCFG::SCFG::SCFG(std::string_view path)
{
	// Load cfg with file_manager

	typeMap.emplace(Type::List::Int, std::make_unique<TypeEntryBase>(TypeEntry<int>(4, Type::LoadType<int>, Type::SafeType<int>)));
}
