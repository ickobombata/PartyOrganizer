#pragma once

#include "LoggingService.hpp"
#include "ConfigurationService.h"
#include "ServiceProvider.hpp"
#include "OrmDefenitions.h"
#include "../Repositories/RAMUserRepository.h"
#include <unordered_map>
#include <string>

using namespace sl; // Silicon namespace
using namespace s; // symbols namespace

typedef mysql_orm<User> user_orm;

//#define USE_DB

class DatabaseService
{
public:
	DatabaseService() : users(nullptr)
	{
		logger = ServiceProvider::Instance().Resolve<LoggingService>();
		configuration = ServiceProvider::Instance().Resolve<ConfigurationService>();

		const char* debug = configuration->Get("Database", "Debug");
		if (!strcmp(debug, "true"))
			users = new RAMUserRepository();
	}


public:
	auto CreateUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db) 
		{
			int id = users->CreateUser(params.username, params.password, params.alias);
			if (id == IUserRepository::ID_NOT_FOUND) 
			{
				return D(_status = "fail");
			}

			logger->Info("Created User with id %d", id);

			return D(_status = "ok");
		};
	}
	auto DeleteUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			bool deleted = users->DeleteUser(params.username);
			if (!deleted) 
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}
	auto EditUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			int id = users->EditUser(params.username, params.password, params.alias);
			if (id == IUserRepository::ID_NOT_FOUND) 
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}
	auto GetUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			User* user = nullptr;
			bool exists = users->GetUser(params.username);
			if (!exists) 
			{
				return D(_status = "fail", _username = params.username, _alias = std::string(""));
			}

			return D(_status = "ok", _username = params.username, _alias = user->alias);
		};
	}

private:
	std::shared_ptr<LoggingService> logger;
	std::shared_ptr<ConfigurationService> configuration;
	IUserRepository* users;
}; 
