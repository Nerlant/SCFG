#pragma once

#include "int_type.h"

namespace SCFG
{
	class TypeEntryBase
	{
	public:
		explicit TypeEntryBase(const uint16_t size)
			: size(size)
		{}

		[[nodiscard]] uint16_t GetSize() const
		{
			return size;
		}
		
	protected:
		uint16_t size; // TODO: Do we need the size to be stored here?
	};

	template <class T>
	class TypeEntry : public TypeEntryBase
	{
	public:
		TypeEntry(const uint16_t size, Type::LoadType_t<T> load_type, Type::SafeType_t<T> safe_type) :
			TypeEntryBase(size), LoadType(std::move(load_type)), SafeType(std::move(safe_type))
		{}


		Type::LoadType_t<T> LoadType;
		Type::SafeType_t<T> SafeType;
	};
}
