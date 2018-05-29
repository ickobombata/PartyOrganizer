#include "TokenService.h"

#include <jwt/jwt_all.h>
#include <chrono>

TokenService::TokenService(const char* secret)
{
	strcpy(this->secret, secret);
}

std::string TokenService::GenerateToken(const std::string& username, const std::string& password)
{
	using json = nlohmann::json;

	// Setup a signer
	HS256Validator signer(secret);

	std::chrono::nanoseconds now = std::chrono::system_clock::now().time_since_epoch();
	now += std::chrono::hours(1);

	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now);
	json payload = { { "sub", "subject" },{ "exp", seconds.count() } };

	// Let's encode the token to a string
	std::string token = JWT::Encode(signer, payload);

	printf("Token %s\n", token.c_str());
	return token;
}
