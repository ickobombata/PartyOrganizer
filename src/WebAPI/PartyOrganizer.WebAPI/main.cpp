#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>
#include <silicon/middlewares/mysql_connection.hh>
#include <silicon/middlewares/mysql_orm.hh>
#include "symbols.hh"
#include <cstdio>

#include "Services/TokenService.h"
#include "Services/LoggingService.h"

TokenService tokenService("ubersecret");

using namespace sl; // Silicon namespace
using namespace s; // Symbols namespace

typedef decltype(D(_username(_primary_key) = std::string(),
	_user_password = std::string(),
	_alias = std::string()))
	user;

typedef mysql_orm_factory<user> user_orm_factory;
typedef mysql_orm<user> user_orm;

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
		= [](auto params, user_orm& orm, mysql_connection& db) {
		int names_count;
		db("SELECT COUNT(*) from users WHERE username = ?")(params.username) >> names_count;
		
		if (names_count > 0) {
			throw error::bad_request("Username '", params.username, "' already exists");
		}

		user new_user;
		new_user.username = params.username;
		new_user.user_password = params.password;
		new_user.alias = params.alias;
		
		int id = orm.insert(new_user);
		return D(_id = id);
	},

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
		= [](auto params) { return D(_token = tokenService.GenerateToken(params.username, params.password)); },

	POST / _token / _validate * post_parameters(_token)
		= [](auto params) { return D(_valid = tokenService.ValidateToken(params.token)); }
);

int main()
{
	setbuf(stdout, NULL);
	
	LoggingService logger("log.txt");

	logger.Info("Starting server...");
	logger.Info("[{}] Starting server...", "Web API");
	logger.Info("[{}] Starting server... {}", "Web API", "Started");
		printf("Starting server...");

		auto middlewares = std::make_tuple(
			mysql_connection_factory("localhost", "kurendo", "kurendo", "party_organizer"),
			user_orm_factory("users")
	);
		
	// Serve hello_api via microhttpd using the json format:
	sl::mhd_json_serve(hello_api, middlewares, 12345);
}
