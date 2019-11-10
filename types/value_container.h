#pragma once

#include "type.h"

#include <utility>
#include <vector>


namespace SCFG
{
	class ValueContainerBase
	{
	public:
		virtual ~ValueContainerBase() = default;
		virtual std::vector<char> Safe() = 0;
		virtual void Load(char* data) = 0;
	};
	
	template<class T>
	class ValueContainer final : public ValueContainerBase
	{
	public:
		explicit ValueContainer() :	// TODO: is this type index needed?
			value()
		{}

		// TODO: make this properly
		explicit ValueContainer(T value) : value(value)
		{}

		//void RegisterChildren(std::unique_ptr<ValueContainer> children)
		//{
		//	childrenTypes.push_back(children);
		//}

		std::vector<char> Safe() override
		{
			return Type::SafeType<T>(value);
		}

		void Load(char* data) override
		{
			value = Type::LoadType<T>(data);
		}

		const T& GetValue()
		{
			return value;
		}

	private:
		T value;
		//std::vector<std::unique_ptr<ValueContainer>> childrenTypes;
	};
}
