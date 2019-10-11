#pragma once

#include "../types/value_container.h"

#include <vector>
#include <memory>
#include <string_view>
#include <map>

// TODO: implement

// each profile must contain:
// map from name to index - maybe not needed but we need to safe the order how the values are saved somewhere (do we?)
// map from index to typeContainer
//
// maybe map from name to typeContainer is enough

namespace SCFG
{
	class SCFG;
	
	class Profile
	{
		// Needs to contain all the settings
	public:
		explicit Profile(std::string_view name, SCFG& scfg);

		template <class T>
		T GetValueByName(const std::string_view name)
		{
			if (const auto val = valueMap.at(name).get())
				return dynamic_cast<ValueContainer<T>*>(val)->GetValue();

			/* Get Value from cfg and cache it
			 *
			 * for that we need: the name list, access to the file manager
			 */
			return T();

			// throw exception if value not found
		}

		template <class T>
		bool SetValue(const std::string_view name, const T& value)
		{
			return valueMap.emplace(name, std::make_shared<ValueContainer<T>>(value)).second;
		}

		std::vector<char> WriteProfile();

	private:
		std::string_view profileName;
		std::map<std::string_view, std::shared_ptr<ValueContainerBase>> valueMap;
		SCFG& scfg;
	};
}
