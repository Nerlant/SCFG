#pragma once

#include "types/type.h"
#include "types/type_list.h"

#include <unordered_map>
#include <string_view>


namespace SCFG
{
	class TypeEntryBase
	{
	};
	
	template <class T>
	class TypeEntry : public TypeEntryBase
	{
	public:
		TypeEntry(size_t size, Type::LoadType_t<T> load_type, Type::SafeType_t<T> safe_type) :
		Size(size), LoadType(std::move(load_type)), SafeType(std::move(safe_type))
		{}
		
		size_t Size;
		Type::LoadType_t<T> LoadType;
		Type::SafeType_t<T> SafeType;
	};
	
	class SCFG
	{
	public:
		SCFG() = delete;
		SCFG(std::string_view path);
		SCFG(const SCFG& o) = delete;
		SCFG(SCFG&& o);

		~SCFG();

		void LoadConfig();
		void SaveProfile();

		template<class T>
		T GetValueByName(const std::string_view name);

		template<class T>
		void SetValue(const std::string_view key, const T& value);

	private:
		std::unordered_map<Type::List, std::unique_ptr<TypeEntryBase>> typeMap;

		// each profile must contain:
		// map from name to index - maybe not needed but we need to safe the order how the values are saved somewhere (do we?)
		// map from index to typeContainer
		//
		// maybe map from name to typeContainer is enough
	};
}
