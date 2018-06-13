#pragma once

#include "../Configuration/SimpleIni.h"
#include "LoggingService.hpp"

class ConfigurationService
{
public:
	ConfigurationService();
	bool Load(const char *);

public:
	const char* Get(const char *, const char *) const;
	void Set(const char *, const char *, const char *);
	
private:
	CSimpleIni ini;
	std::shared_ptr<LoggingService> logger;
};
