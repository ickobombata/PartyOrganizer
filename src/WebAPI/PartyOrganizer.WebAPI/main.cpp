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

typedef mysql_orm_factory<user> user_orm_factory;

DatabaseService databaseService;
				   // Define the API:
auto hello_api = http_api(

	// The hello world procedure.
	GET / _hello = []() { return D(_message = "Hello world."); },

	// example of the database data retrieval
	GET / _username = [](auto p, mysql_connection& db) {
		std::string name;
		db("SELECT username from users where alias = ?")("Due") >> name;
		return D(_username = name);
	},

	GET / _create_user * get_parameters(_username = std::string(), _password = std::string(), _alias = std::string())
		= databaseService.create_user(_username, _password, _alias),

GET / _delete_user * get_parameters(_username = std::string())
= [](auto params, user_orm& orm, mysql_connection& db) {
	auto r = D(_username = std::string(), _user_password = std::string(), _alias = std::string());


	std::string alias; std::string name, pass;
	db("SELECT * from users WHERE username = ? LIMIT 1")(params.username) >> std::tie(name, pass, alias);

	if (name == "") {
		throw error::not_found("Username '", params.username, "' does not exists");
	}

	user u;
	u.username = name;
	u.user_password = pass;
	u.alias = alias;

	orm.destroy(u);

	return(D(_alias = alias, _username = name, _password = pass));
},
// TODO stoevm: remove code duplication!!!
GET / _update_user * get_parameters(_username = std::string(),
	_password = std::string(), _alias = std::string())
	= [](auto params, user_orm& orm, mysql_connection& db) {

	std::string alias; std::string name, pass;
	db("SELECT * from users WHERE username = ? LIMIT 1")(params.username) >> std::tie(name, pass, alias);

	if (name == "") {
		throw error::not_found("Username '", params.username, "' does not exists");
	}

	user u;
	u.username = name;
	u.user_password = params.password;
	u.alias = params.alias;

	int id = orm.update(u);

	return D(_id = id);
},

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
}

int main()
{
	setbuf(stdout, NULL);

	InitializeServices();

	auto logger = ServiceProvider::Instance().Resolve<LoggingService>();

	logger->Info("Loading middlewares...");

	auto middlewares = std::make_tuple(
		mysql_connection_factory("localhost", "kurendo", "kurendo", "party_organizer"),
		user_orm_factory("users")
	);

	logger->Info("Server started.");
	// Serve hello_api via microhttpd using the json format:
	sl::mhd_json_serve(hello_api, middlewares, 12345);
}
