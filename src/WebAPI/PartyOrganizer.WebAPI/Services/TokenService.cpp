#include "TokenService.h"

#include <chrono>

TokenService::TokenService(const char* secret) : signer(secret)
{

}

std::string TokenService::GenerateToken(const std::string& username, const std::string& password)
{
	using json = nlohmann::json;

	std::chrono::nanoseconds now = std::chrono::system_clock::now().time_since_epoch();
	now += std::chrono::hours(1);

	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now);
	json payload = { { "sub", "subject" },{ "exp", seconds.count() } };

	// Let's encode the token to a string
	std::string token = JWT::Encode(signer, payload);

	printf("Token %s\n", token.c_str());
	return token;
}

bool TokenService::ValidateToken(const std::string& token)
{
	try
	{
		ExpValidator exp;
		json header, payload;

		std::tie(header, payload) = JWT::Decode(token, &signer, &exp);
		return true;
	}
	catch (InvalidTokenError& ex)
	{
		return false;
	}
}
