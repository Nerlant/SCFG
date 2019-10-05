#pragma once

#include "type_list.h"

#include <utility>
#include <vector>


namespace SCFG
{
	template<class T>
	class TypeContainer
	{
	public:
		TypeContainer(const Type::List index, Type::SafeType_t<T> safe_type, Type::LoadType_t<T> load_type) :
			safeType(std::move(safe_type)), loadType(std::move(load_type)), value(), index(index)
		{}

		//void RegisterChildren(std::unique_ptr<TypeContainer> children)
		//{
		//	childrenTypes.push_back(children);
		//}

		std::vector<char> Safe()
		{
			return safeType(value);
		}

		void Load(char* data)
		{
			value = loadType(data);
		}

		const T& GetValue()
		{
			return value;
		}

		[[nodiscard]] Type::List GetTypeIndex() const
		{
			return index;
		}

	private:
		Type::SafeType_t<T> safeType;
		Type::LoadType_t<T> loadType;

		T value;
		Type::List index;
		//std::vector<std::unique_ptr<TypeContainer>> childrenTypes;
	};
}
