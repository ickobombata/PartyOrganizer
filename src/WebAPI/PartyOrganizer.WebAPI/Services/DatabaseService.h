#pragma once

#include "LoggingService.hpp"
#include "ServiceProvider.hpp"
#include "OrmDefenitions.h"
#include <unordered_map>
#include <string>

using namespace sl; // Silicon namespace
using namespace s; // symbols namespace

typedef mysql_orm<User> user_orm;

//#define USE_DB

class DatabaseService
{
private:
	std::shared_ptr<LoggingService> logger;
public:
	DatabaseService() {
		logger = ServiceProvider::Instance().Resolve<LoggingService>();
	}

#ifdef USE_DB
	auto CreateUser() {
		return [this](auto params, user_orm& orm, mysql_connection& db) {
			int namesCount;
			db("SELECT COUNT(*) from Users WHERE username = ?")(params.username) >> namesCount;

			if (namesCount > 0) {
				throw error::bad_request("Username '", params.username, "' already exists");
			}

			User user;
			user.username = params.username;
			user.password = params.password;
			user.alias = params.alias;

			int id = orm.insert(user);
			this->logger->Info("Created User with id %d", id);

			return D(_id = id);
		};
	}

	auto DeleteUser() {
		return [](auto params, user_orm& orm, mysql_connection& db) 
		{
			std::string alias; std::string name, pass;
			db("SELECT * from Users WHERE username = ? LIMIT 1")(params.username) >> std::tie(name, pass, alias);

			if (name == "")
			{
				throw error::not_found("Username '", params.username, "' does not exists");
			}

			User user;
			user.username = name;
			user.password = pass;
			user.alias = alias;

			orm.destroy(user);

			return D(_alias = alias, _username = name, _password = pass);
		};
	}

	auto EditUser() {
		return [](auto params, user_orm& orm, mysql_connection& db) 
		{
			std::string alias; std::string name, pass;
			db("SELECT * from Users WHERE username = ? LIMIT 1")(params.username) >> std::tie(name, pass, alias);

			if (name == "") 
			{
				throw error::not_found("Username '", params.username, "' does not exists");
			}

			User user;
			user.username = name;
			user.password = params.password;
			user.alias = params.alias;

			int id = orm.update(user);

			return D(_id = id);
		};
	}

	auto GetUser() {
		return [](auto params, mysql_connection& db) 
		{
			std::string name, alias;
			db("SELECT username, alias from Users where username = ?")(params.username) >> std::tie(name, alias);
			if (name == "") 
			{
				throw error::bad_request("Username '", params.username, "' does not exists");
			}
			return D(_username = name, _alias = alias);
		};
	}
#endif

#ifndef USE_DB
private:
	std::unordered_map<std::string, User*> users;
	std::unordered_map<std::string, Multimedia> multimedia;
	std::unordered_map<std::string, Task> tasks;
	std::unordered_map<std::string, Message> messages;
	std::unordered_map<std::string, Chat> chats;
	std::unordered_map<std::string, Status> statuses;
	std::unordered_map<std::string, Event> events;

public:
	auto CreateUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db) {
			if (this->users.find(params.username) != this->users.end()) {
				return D(_status = "fail");
			}

			User* user = new User();
			user->username = params.username;
			user->password = params.password;
			user->alias = params.alias;

			this->users[params.username] = user;

			logger->Info("Created User with id %d", this->users.size());

			return D(_status = "ok");
		};
	}
	auto DeleteUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			if (this->users.find(params.username) == this->users.end()) {
				return D(_status = "fail");
			}
			this->users.erase(params.username);

			return D(_status = "ok");
		};
	}
	auto EditUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			if (this->users.find(params.username) == this->users.end()) {
				return D(_status = "fail");
			}
			
			this->users[params.username]->password = params.password;
			this->users[params.username]->alias = params.alias;

			return D(_status = "ok");
		};
	}
	auto GetUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			if (this->users.find(params.username) == this->users.end()) {
				return D(_status = "fail", _username = params.username, _alias = std::string(""));
			}

			return D(_status = "ok", _username = params.username, _alias = this->users.find(params.username)->second->alias);
		};
	}
#endif
}; 