#pragma once

#include "Services/LoggingService.hpp"
#include "Services/ConfigurationService.h"
#include "Services/TokenService.h"
#include "Services/DatabaseService.h"

using namespace sl; // Silicon namespace
using namespace s; // Symbols namespace

class ServiceInitializer 
{
public:
	static void InitializeServices()
	{
		std::shared_ptr<LoggingService> loggingService = std::make_shared<LoggingService>();
		loggingService->RegisterLogFile("WebAPI", "log.txt");
		loggingService->RegisterConsole("Console");

		ServiceProvider::Instance().Register(loggingService);
		
		std::shared_ptr<ConfigurationService> configurationService = std::make_shared<ConfigurationService>();
		if (!configurationService->Load("config.ini"))
			throw std::runtime_error("Could not load configuration");

		ServiceProvider::Instance().Register(configurationService);
		
		std::string secret = configurationService->Get("Auth", "Key");
		std::shared_ptr<TokenService> tokenService = std::make_shared<TokenService>(secret);

		ServiceProvider::Instance().Register(tokenService);

		std::shared_ptr<DatabaseService> databaseService = std::make_shared<DatabaseService>();
		ServiceProvider::Instance().Register(databaseService);
	}

	static auto GetApis()
	{
		return http_api(
			// The hello world procedure.
			GET / _hello = []() { return D(_message = "Hello world."); },

			//// example of the database data retrieval
			GET / _get_user * get_parameters(_username = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->GetUser(),

			GET / _create_user * get_parameters(_username = std::string(), _password = std::string(), _alias = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->CreateUser(),

			GET / _delete_user * get_parameters(_username = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->DeleteUser(),

			GET / _update_user * get_parameters(_username = std::string(), _password = std::string(), _alias = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->EditUser(),



			GET / _get_event * get_parameters(_roomName = std::string(), _eventName = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->GetEvent(),

			GET / _create_event * get_parameters(_roomName = std::string(), _eventName = std::string(), _dateStarts = std::string(), _dateEnds = std::string(), _dateCreated = std::string(), _minUsers = int(), _maxUsers = int())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->CreateEvent(),

			GET / _delete_event * get_parameters(_roomName = std::string(), _eventName = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->DeleteEvent(),

			GET / _update_event * get_parameters(_roomName = std::string(), _eventName = std::string(), _dateStarts = std::string(), _dateEnds = std::string(), _dateCreated = std::string(), _minUsers = int(), _maxUsers = int())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->EditEvent(),



			GET / _get_eventTask * get_parameters(_eventName = std::string(), _eventTaskName = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->GetEventTask(),

			GET / _create_eventTask * get_parameters(_eventName = std::string(), _eventTaskName = std::string(), _description = std::string(), _complexity = int(), _state = int())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->CreateEventTask(),

			GET / _delete_eventTask * get_parameters(_eventName = std::string(), _eventTaskName = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->DeleteEventTask(),

			GET / _update_eventTask * get_parameters(_eventName = std::string(), _eventTaskName = std::string(), _description = std::string(), _complexity = int(), _state = int())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->EditEventTask(),




			GET / _create_room * get_parameters(_name = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->CreateRoom(),

			GET / _delete_room * get_parameters(_name = std::string())
			= ServiceProvider::Instance().Resolve<DatabaseService>()->DeleteRoom(),

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
};