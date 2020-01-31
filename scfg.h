#pragma once

#include "file_manager/file_manager.h"
#include "profile/profile.h"
#include "header.h"
#include "types/type_includes.h"

#include <unordered_map>
#include <string_view>
#include <memory>


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
		T GetValueByName(const std::string& name)
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
					SetValue(name, T());

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
				const auto lastFieldInfo = typeMap.rbegin()->second;
				typeMap.emplace(key, FieldInfo(static_cast<uint32_t>(sizeof(T)),
				                               lastFieldInfo.FileOffset + lastFieldInfo.FileSize,
				                               Type::GetTypeSize<T>()));
				
				// Insert new key with default value into all profiles
				for (auto& [profileName, profile] : profileMap)
				{
					if (profileName == currentProfile)
						continue;
					profile.SetValue(key, T{});
				}
			}
		}

	private:
		std::map<std::string, FieldInfo, std::less<>> typeMap;
		std::unordered_map<std::string, Profile> profileMap;
		FileManager fm;
		std::string currentProfile;

		void writeHeader();
		size_t writeProfileMap(size_t offset, uint32_t profile_offset);
		size_t writeTypeMap(size_t offset);

		[[nodiscard]] std::unique_ptr<Header> loadHeader() const;
		size_t readProfileMap(uint16_t number_of_profiles, size_t offset);
		size_t readTypeMap(uint16_t number_of_types, size_t offset);

		void checkTypeSize(std::string_view field_name, size_t field_size) const;
	};
}
