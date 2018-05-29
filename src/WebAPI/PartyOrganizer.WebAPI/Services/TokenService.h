#pragma once

#include <string>

class TokenService
{
private:
	static const int SECRET_LENGTH = 64;

public:
	TokenService(const char *);

public:
	std::string GenerateToken(const std::string &, const std::string &);

private:
	char secret[SECRET_LENGTH];
};
