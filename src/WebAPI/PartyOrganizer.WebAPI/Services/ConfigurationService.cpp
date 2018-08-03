#include "ConfigurationService.h"
#include "ServiceProvider.hpp"

ConfigurationService::ConfigurationService() : logger(nullptr)
{
	logger = ServiceProvider::Instance().Resolve<LoggingService>();
	logger->Info("Configuration service initialized.");
}

bool ConfigurationService::Load(const char* filePath)
{
	SI_Error result = ini.LoadFile(filePath);
	if (result >= 0)
		logger->Info("Loaded configuration from file {}", filePath);

	return result >= 0;
}

std::string ConfigurationService::Get(const char* section, const char* key) const
{
	return ini.GetValue(section, key);
}

void ConfigurationService::Set(const char* section, const char* key, const char* newValue)
{
	ini.SetValue(section, key, newValue);
} 
