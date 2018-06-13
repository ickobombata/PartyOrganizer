#include "ConfigurationService.h"

bool ConfigurationService::Load(const char* filePath)
{
	SI_Error result = ini.LoadFile("config.ini");

	return result >= 0;
}

const char* ConfigurationService::Get(const char* section, const char* key) const
{
	return ini.GetValue(section, key);
}

void ConfigurationService::Set(const char* section, const char* key, const char* newValue)
{
	ini.SetValue(section, key, newValue);
} 
