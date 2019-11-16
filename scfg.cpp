#include "scfg.h"
#include "header.h"
#include "exceptions.h"

constexpr uint32_t CFG_MAGIC = 0x47464353;
constexpr uint8_t NAME_MAX_LENGTH = UINT8_MAX;

SCFG::SCFG::SCFG(const std::string_view path) : fm(path) // TODO: add bool to create default config
{
	// Load cfg with file_manager
	fm.Load();

	//typeMap.emplace(Type::List::Int, std::make_unique<TypeEntryBase>(TypeEntry<int>(4, Type::LoadType<int>, Type::SaveType<int>)));

	loadHeader();

	// TODO: add number of profiles to header
	// maybe put profile name as list behind number - maybe add file offset (to profile?)

	// read name list
	
	// read first profile, if none exists, create a default one

	//TODO remove
	profileMap.emplace("p1", Profile("p1", *this));
	currentProfile = "p1";
}

SCFG::SCFG::~SCFG()
{
}

void SCFG::SCFG::SafeAsNewProfile(std::string_view name)
{
	profileMap.emplace(name, profileMap.at(currentProfile));
}

void SCFG::SCFG::WriteConfig()
{
	writeHeader();

	// TODO: write profile structure
	// TODO: write all profiles 
}

void SCFG::SCFG::SaveConfig()
{
	WriteConfig();
	fm.Safe();
}

void SCFG::SCFG::writeHeader()
{
	const auto headerPtr = std::make_unique<Header>();
	headerPtr->CharacteristicBytes = CFG_MAGIC;
	headerPtr->NumberOfProfiles = static_cast<uint16_t>(profileMap.size());
	headerPtr->NumberOfFields = static_cast<uint16_t>(typeMap.size());
	fm.Write<Header>(0, *headerPtr);

	// TODO: put in own function
	auto fileOffset = sizeof(Header);
	std::vector<char> name(NAME_MAX_LENGTH);
	
	for (auto&& [entryName, val] : typeMap)
	{
		std::fill(name.begin(), name.end(), '\0');
		name.insert(name.begin(), entryName.begin(), entryName.end() - sizeof(char)); // Last character is '\0'
		fm.Write(fileOffset, entryName.data(), entryName.size() + sizeof(char));
		
		fileOffset += entryName.size() + sizeof(char);
		fm.Write<uint32_t>(fileOffset, val);
		fileOffset += sizeof val;
	}
}

void SCFG::SCFG::loadHeader()
{
	const auto headerPtr = std::make_unique<Header>();

	headerPtr->CharacteristicBytes = fm.Read<int>(0);
	if (headerPtr->CharacteristicBytes != CFG_MAGIC)
		throw Exception::InvalidConfigFileException();

	headerPtr->NumberOfProfiles = fm.Read<uint16_t>(4);
	headerPtr->NumberOfFields = fm.Read<uint16_t>(6);

	// TODO: put in own function
	auto fileOffset = sizeof(Header);
	for (uint16_t i = 0; i < headerPtr->NumberOfFields; i++)
	{
		const std::string name(fm.Read(fileOffset, NAME_MAX_LENGTH).data());
		fileOffset += name.size() + sizeof(char); // Add one for the null-character
		
		const auto typeSize = fm.Read<uint32_t>(fileOffset);
		fileOffset += sizeof typeSize;

		// Store type name and type length to be able to later detect type mismatches
		if (typeMap.contains(name))
			throw Exception::DuplicateFieldNameException(name);
		
		typeMap.emplace(name, typeSize);
	}
}

/// <summary>
/// Checks the size of the type.
/// Throws SizeMismatchException if field_size does not match.
/// Throws InvalidFieldNameException if field_name does not exist.
/// </summary>
/// <param name="field_name">Name of the field.</param>
/// <param name="field_size">Size of the field.</param>
void SCFG::SCFG::checkTypeSize(const std::string_view field_name, const size_t field_size)
{
	try
	{
		if (typeMap.at(field_name.data()) != field_size)
			throw Exception::SizeMismatchException();
	}
	catch (const std::out_of_range & ex)
	{
		throw Exception::InvalidFieldNameException(ex.what());
	}
}
