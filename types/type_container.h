#pragma once

#include "type_list.h"

#include <functional>
#include <vector>
#include <memory>

namespace SCFG
{
	template<class T>
	class TypeContainer
	{
	public:
		TypeContainer(const Type::List index, std::function<void(const T&, char*)>&& safe_type, std::function<T(char*)>&& load_type) :
			safeType(safe_type), loadType(load_type), value(), index(index)
		{}

		//void RegisterChildren(std::unique_ptr<TypeContainer> children)
		//{
		//	childrenTypes.push_back(children);
		//}

		void Safe(char* data)
		{
			safeType(value, data);
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
		std::function<void(const T&, char*)> safeType;
		std::function<T(char*)> loadType;

		T value;
		Type::List index;
		//std::vector<std::unique_ptr<TypeContainer>> childrenTypes;
	};
}
