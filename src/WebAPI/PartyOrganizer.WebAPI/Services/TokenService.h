#pragma once

#include <string>
#include <jwt/jwt.hpp>

class TokenService
{
public:
	TokenService(const char *);

public:
	std::string GenerateToken(const std::string &, const std::string &);
	bool ValidateToken(const std::string &);

private:
	char signingSecret[64];
};
