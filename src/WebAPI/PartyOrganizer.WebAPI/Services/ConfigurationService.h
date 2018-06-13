#pragma once

#include "../Configuration/SimpleIni.h"

class ConfigurationService
{
public:
	bool Load(const char *);

public:
	const char* Get(const char *, const char *) const;
	void Set(const char *, const char *, const char *);
	
private:
	CSimpleIni ini;
};
