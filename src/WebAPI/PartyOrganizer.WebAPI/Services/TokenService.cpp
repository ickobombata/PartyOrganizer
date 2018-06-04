#include "TokenService.h"

#include <chrono>

char ISSUER[] = "Party Organizer";

TokenService::TokenService(const char* secret)
{
	strcpy(signingSecret, secret);
}

std::string TokenService::GenerateToken(const std::string& username, const std::string& password)
{
	using namespace jwt::params;

	jwt::jwt_object obj{ algorithm("hs256"), secret(signingSecret), payload({}) };
	obj.payload().add_claim("user", username);
	obj.payload().add_claim("iss", ISSUER);
	obj.payload().add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{ 3600 });

	std::string token = obj.signature();

	printf("Token %s\n", token.c_str());
	return token;
}

bool TokenService::ValidateToken(const std::string& token)
{
	using namespace jwt::params;

	try
	{
		jwt::jwt_object obj = jwt::decode(token, algorithms({ "hs256" }), secret(signingSecret), issuer(ISSUER));
	}
	catch (const jwt::TokenExpiredError& e)
	{
		return false;
	}
	catch (const jwt::InvalidIssuerError& e)
	{
		return false;
	}
	catch (const std::runtime_error& e)
	{
		const char* wut = e.what();
		return false;
	}

	return true;
}
