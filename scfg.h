#pragma once

#include "types/type_entry.h"
#include "file_manager/file_manager.h"
#include "profile/profile.h"

#include <unordered_map>
#include <string_view>
#include <algorithm>


namespace SCFG
{
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
		void SafeAsNewProfile(std::string_view name);

		void WriteConfig();
		void SaveConfig();

		template<class T>
		T GetValueByName(const std::string_view name)
		{
			try
			{
				checkTypeSize(name, sizeof(T));
				
				return profileMap.at(currentProfile).GetValueByName<T>(name);
			}
			catch (const Exception::InvalidFieldNameException&)
			{
				// Insert new value in all profiles
				for (auto& [key, profile] : profileMap)
					profile.SetValue<T>(name, T{});

				return T{};
			}
		}

		template<class T>
		void SetValue(const std::string_view key, const T& value)
		{
			checkTypeSize(key, sizeof(T));
			
			if (profileMap.at(currentProfile).SetValue<T>(key, value))
			{
				profileStructure.push_back(key);
				std::sort(profileStructure.begin(), profileStructure.end());
				
				// Insert new key with default value into all profiles
				for (auto& [profileName, profile] : profileMap)
					profile.SetValue(key, T{});
			}
		}

	private:
		std::vector<std::string_view> profileStructure;
		std::map<std::string, uint32_t> typeMap;
		std::unordered_map<std::string_view, Profile> profileMap;
		FileManager fm;
		std::string_view currentProfile;

		void writeHeader();
		void loadHeader();

		void checkTypeSize(std::string_view field_name, size_t field_size);
	};
}
