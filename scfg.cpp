#include "scfg.h"
#include "header.h"
#include "exceptions.h"

#include <memory>

constexpr uint32_t CFG_MAGIC = 0x47464353;
constexpr uint8_t NAME_MAX_LENGTH = UINT8_MAX;

SCFG::SCFG::SCFG(const std::string_view path) : fm(path) // TODO: add bool to create default config
{
	// Load cfg with file_manager
	fm.Load();

	LoadConfig();
	
	// read first profile, if none exists, create a default one

	currentProfile = "test123";
}

SCFG::SCFG::~SCFG()
{
}

void SCFG::SCFG::LoadConfig()
{
	const auto headerPtr = loadHeader();

	const auto offset = readProfileMap(headerPtr->NumberOfProfiles, sizeof(Header));
	readTypeMap(headerPtr->NumberOfFields, offset);
}

void SCFG::SCFG::SafeAsNewProfile(std::string_view name)
{
	profileMap.emplace(name, profileMap.at(currentProfile));
}

void SCFG::SCFG::WriteConfig()
{
	writeHeader();

	auto offset = sizeof(Header);
	// Calculate size of profile list in file
	for (const auto& [profileName, profile] : profileMap)
		offset += profileName.size() + sizeof(char) + sizeof(uint32_t);

	const auto firstProfileOffset = static_cast<uint32_t>(writeTypeMap(offset));
	writeProfileMap(sizeof(Header), firstProfileOffset);
}

void SCFG::SCFG::SaveConfig()
{
	WriteConfig();
	fm.Save();
}

void SCFG::SCFG::writeHeader()
{
	const auto headerPtr = std::make_unique<Header>();
	headerPtr->CharacteristicBytes = CFG_MAGIC;
	headerPtr->NumberOfProfiles = static_cast<uint16_t>(profileMap.size());
	headerPtr->NumberOfFields = static_cast<uint16_t>(typeMap.size());
	fm.Write<Header>(0, *headerPtr);
}

size_t SCFG::SCFG::writeProfileMap(size_t offset, uint32_t profile_offset)
{
	std::vector<char> name(NAME_MAX_LENGTH);

	for (const auto& [profileName, profile] : profileMap)
	{
		std::fill(name.begin(), name.end(), '\0');
		name.insert(name.begin(), profileName.begin(), profileName.end());
		
		fm.Write(offset, name.data(), profileName.size() + sizeof(char));
		offset += profileName.size() + sizeof(char);

		fm.Write(offset, profile_offset);
		offset += sizeof profile_offset;

		profile_offset += static_cast<uint32_t>(writeProfile(profile_offset, profile));
	}

	return offset;
}

size_t SCFG::SCFG::writeProfile(const size_t offset, const Profile& profile)
{
	const auto profileData = profile.GetData();
	fm.Write(offset, profileData.data(), profileData.size());
	return profileData.size();
}

size_t SCFG::SCFG::writeTypeMap(size_t offset)
{	
	std::vector<char> name(NAME_MAX_LENGTH);

	for (const auto& [entryName, val] : typeMap)
	{
		std::fill(name.begin(), name.end(), '\0');
		name.insert(name.begin(), entryName.begin(), entryName.end());
		
		fm.Write(offset, name.data(), entryName.size() + sizeof(char));
		offset += entryName.size() + sizeof(char);
		
		fm.Write<uint32_t>(offset, val);
		offset += sizeof val;
	}

	return offset;
}

std::unique_ptr<SCFG::Header> SCFG::SCFG::loadHeader() const
{
	auto headerPtr = std::make_unique<Header>();

	headerPtr->CharacteristicBytes = fm.Read<int>(0);
	if (headerPtr->CharacteristicBytes != CFG_MAGIC)
		throw Exception::InvalidConfigFileException();

	headerPtr->NumberOfProfiles = fm.Read<uint16_t>(4);
	headerPtr->NumberOfFields = fm.Read<uint16_t>(6);

	return headerPtr;
}

size_t SCFG::SCFG::readProfileMap(const uint16_t number_of_profiles, size_t offset)
{
	for (uint16_t i = 0; i < number_of_profiles; i++)
	{
		const std::string name(fm.Read(offset, NAME_MAX_LENGTH).data());
		offset += name.size() + sizeof(char); // Add one for the null-character

		const auto fileOffset = fm.Read<uint32_t>(offset);
		offset += sizeof fileOffset;

		if (profileMap.contains(name))
			throw Exception::DuplicateProfileNameException(name);

		profileMap.emplace(name, Profile(fileOffset, fm, typeMap));
	}

	return offset;
}

size_t SCFG::SCFG::readTypeMap(const uint16_t number_of_types, size_t offset)
{
	for (uint16_t i = 0; i < number_of_types; i++)
	{
		const std::string name(fm.Read(offset, NAME_MAX_LENGTH).data());
		offset += name.size() + sizeof(char); // Add one for the null-character

		const auto typeSize = fm.Read<uint32_t>(offset);
		offset += sizeof typeSize;

		// Store type name and type length to be able to later detect type mismatches
		if (typeMap.contains(name))
			throw Exception::DuplicateFieldNameException(name);

		typeMap.emplace(name, typeSize);
	}

	return offset;
}

/**
 * \brief Check the size of the type.
 * \param field_name Name of the field.
 * \param field_size Size of the field.
 * \throws SizeMismatchException if field_size does not match.
 * \throws InvalidFieldNameException if field_name does not exist.
 */
void SCFG::SCFG::checkTypeSize(const std::string_view field_name, const size_t field_size) const
{
	const auto pair = typeMap.find(field_name);
	if (pair == typeMap.end())
		throw Exception::InvalidFieldNameException(std::string(field_name));
	if (pair->second != field_size)
		throw Exception::SizeMismatchException();
}
