#include "TokenService.h"

#include <jwt/jwt_all.h>

TokenService::TokenService(const char* secret)
{
	strcpy(this->secret, secret);
}

std::string TokenService::GenerateToken(const std::string& username, const std::string& password)
{
	using json = nlohmann::json;

	// Setup a signer
	HS256Validator signer(secret);

	// Create the json payload that expires 01/01/2017 @ 12:00am (UTC)
	json payload = { { "sub", "subject" },{ "exp", 1483228800 } };

	// Let's encode the token to a string
	std::string token = JWT::Encode(signer, payload);

	printf("Token %s\n", token.c_str());
	return token;
}
