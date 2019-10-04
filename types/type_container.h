#pragma once

#include "type_list.h"

#include <functional>
#include <utility>
#include <vector>


namespace SCFG
{
	template<class T>
	class TypeContainer
	{
	public:
		TypeContainer(const Type::List index, std::function<std::vector<char>(const T&)> safe_type,
		              std::function<T(const char*)> load_type) :
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
		std::function<std::vector<char>(const T&)> safeType;
		std::function<T(const char*)> loadType;

		T value;
		Type::List index;
		//std::vector<std::unique_ptr<TypeContainer>> childrenTypes;
	};
}
