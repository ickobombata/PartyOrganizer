#pragma once

#include <string>
#include <jwt/jwt_all.h>

class TokenService
{
public:
	TokenService(const char *);

public:
	std::string GenerateToken(const std::string &, const std::string &);
	bool ValidateToken(const std::string &);

private:
	HS256Validator signer;
};
