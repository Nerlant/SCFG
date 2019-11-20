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
		void SafeAsNewProfile(const std::string& name);

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
			try
			{
				checkTypeSize(key, sizeof(T));
			}
			// It is okay if the field does not exist so we ignore the exception
			catch (const Exception::InvalidFieldNameException&) {}
			
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
		std::unordered_map<std::string, Profile> profileMap;
		FileManager fm;
		std::string currentProfile;

		void writeHeader();
		size_t writeProfileMap(size_t offset, uint32_t profile_offset);
		size_t writeProfile(size_t offset, const Profile& profile);
		size_t writeTypeMap(size_t offset);
		
		void loadHeader();
		size_t readProfileMap(uint16_t number_of_profiles, size_t offset);
		size_t readTypeMap(uint16_t number_of_types, size_t offset);

		void checkTypeSize(std::string_view field_name, size_t field_size);
	};
}
