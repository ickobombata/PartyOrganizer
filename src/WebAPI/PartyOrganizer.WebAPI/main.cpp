#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>
#include <silicon/middlewares/mysql_connection.hh>
#include <silicon/middlewares/mysql_orm.hh>
#include "symbols.hh"
#include <cstdio>

#include "Services/TokenService.h"
#include "Services/LoggingService.hpp"
#include "Services/ServiceProvider.hpp"
#include "Services/ConfigurationService.h"
#include "Services/DatabaseService.h"

using namespace sl; // Silicon namespace
using namespace s; // Symbols namespace

typedef mysql_orm_factory<User> user_orm_factory;


void InitializeServices()
{
	std::shared_ptr<LoggingService> loggingService = std::make_shared<LoggingService>();
	loggingService->RegisterLogFile("WebAPI", "log.txt");
	loggingService->RegisterConsole("Console");

	ServiceProvider::Instance().Register(loggingService);

	std::shared_ptr<ConfigurationService> configurationService = std::make_shared<ConfigurationService>();
	if (!configurationService->Load("config.ini"))
		throw std::runtime_error("Could not load configuration");

	ServiceProvider::Instance().Register(configurationService);

	const char* secret = configurationService->Get("Auth", "Key");
	std::shared_ptr<TokenService> tokenService = std::make_shared<TokenService>(secret);

	ServiceProvider::Instance().Register(tokenService);

	std::shared_ptr<DatabaseService> databaseService = std::make_shared<DatabaseService>();
	ServiceProvider::Instance().Register(databaseService);
}

auto GetApis()
{
	return http_api(
		// The hello world procedure.
		GET / _hello = []() { return D(_message = "Hello world."); },

		// example of the database data retrieval
		GET / _username * get_parameters(_username = std::string())
		= ServiceProvider::Instance().Resolve<DatabaseService>()->GetUser(),

		GET / _create_user * get_parameters(_username = std::string(), _password = std::string(), _alias = std::string())
		= ServiceProvider::Instance().Resolve<DatabaseService>()->CreateUser(),

		GET / _delete_user * get_parameters(_username = std::string())
		= ServiceProvider::Instance().Resolve<DatabaseService>()->DeleteUser(),

		GET / _update_user * get_parameters(_username = std::string(), _password = std::string(), _alias = std::string())
		= ServiceProvider::Instance().Resolve<DatabaseService>()->EditUser(),

		POST / _token / _generate * post_parameters(_username, _password)
		= [](auto params)
	{
		auto tokenService = ServiceProvider::Instance().Resolve<TokenService>();
		return D(_token = tokenService->GenerateToken(params.username, params.password));
	},

		POST / _token / _validate * post_parameters(_token)
		= [](auto params)
	{
		auto tokenService = ServiceProvider::Instance().Resolve<TokenService>();
		return D(_valid = tokenService->ValidateToken(params.token));
	}
	);
}

int main()
{
	setbuf(stdout, NULL);

	InitializeServices();

	// Define the API:
	auto apis = GetApis();

	auto logger = ServiceProvider::Instance().Resolve<LoggingService>();

	logger->Info("Loading middlewares...");

	auto middlewares = std::make_tuple(
		mysql_connection_factory("localhost", "kurendo", "kurendo", "party_organizer"),
		user_orm_factory("Users")
	);

	logger->Info("Server started.");
	// Serve hello_api via microhttpd using the json format:
	sl::mhd_json_serve(apis, middlewares, 12345);
}
