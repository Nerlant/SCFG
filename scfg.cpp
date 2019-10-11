#include "scfg.h"
#include "header.h"
#include "exceptions.h"

constexpr uint32_t IMAGE_MAGIC = 0x47464353;

SCFG::SCFG::SCFG(const std::string_view path) : fm(path) // TODO: add bool to create default config
{
	// Load cfg with file_manager
	fm.Load();

	typeMap.emplace(Type::List::Int, std::make_unique<TypeEntryBase>(TypeEntry<int>(4, Type::LoadType<int>, Type::SafeType<int>)));

	loadHeader();

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

void SCFG::SCFG::SafeConfig()
{
	WriteConfig();
	fm.Safe();
}

void SCFG::SCFG::writeHeader()
{
	const auto headerPtr = std::make_unique<Header>();
	headerPtr->CharacteristicBytes = IMAGE_MAGIC;
	headerPtr->NumberOfTypes = static_cast<int>(Type::List::TypeMax);
	fm.Write<Header>(0, *headerPtr);
	
	uint16_t i = 0;
	for (auto&& [key, val] : typeMap)
	{		
		fm.Write<uint16_t>(6 + i * 4, i);
		fm.Write<uint16_t>(8, val->GetSize());
		i++;
	}
}

void SCFG::SCFG::loadHeader()
{
	const auto headerPtr = std::make_unique<Header>();

	headerPtr->CharacteristicBytes = fm.Read<int>(0);
	if (headerPtr->CharacteristicBytes != IMAGE_MAGIC)
		throw Exception::InvalidConfigFileException();

	headerPtr->NumberOfTypes = fm.Read<uint16_t>(4);

	for (uint16_t i = 0; i < headerPtr->NumberOfTypes; i++)
	{
		const auto typeIndex = static_cast<Type::List>(fm.Read<uint16_t>(6 + i * 4));
		const auto typeSize = fm.Read<uint16_t>(8 + i * 4);
		
		// Compare types to typeMap to detect type mismatches
		try
		{
			if (typeMap.at(typeIndex)->GetSize() != typeSize)
				throw Exception::TypeMismatchException();
		}
		catch (const std::out_of_range& ex)
		{
			throw Exception::InvalidTypeIndexException(ex.what());
		}
	}
}
