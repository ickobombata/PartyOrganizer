#pragma once

#include "LoggingService.hpp"
#include "ServiceProvider.hpp"
#include "OrmDefenitions.h"
#include <vector>

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
	std::vector<User> users;
	std::vector<Multimedia> multimedia;
	std::vector<Task> tasks;
	std::vector<Message> messages;
	std::vector<Chat> chats;
	std::vector<Status> statuses;
	std::vector<Event> events;

public:
	auto CreateUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db) {
			for (int i = 0; i < this->users.size(); ++i)
			{
				if (this->users[i].username == params.username)
				{
					throw error::bad_request("Username '", params.username, "' already exists");
				}
			}

			User user;
			user.username = params.username;
			user.password = params.password;
			user.alias = params.alias;

			this->users.push_back(user);

			logger->Info("Created User with id %d", this->users.size());

			return D(_id = this->users.size());
		};
	}
	auto DeleteUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			int index = -1;
			for (int i = 0; i < this->users.size(); ++i)
			{
				if (this->users[i].username == params.username)
				{
					index = i;
					break;
				}
			}

			if (index >= 0)
			{
				this->users.erase(this->users.begin() + index);
			}
			else
			{
				throw error::not_found("Username '", params.username, "' does not exists");
			}

			return D(_id = index);
		};
	}
	auto EditUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			int index = -1;
			for (int i = 0; i < this->users.size(); ++i)
			{
				if (this->users[i].username == params.username)
				{
					index = i;
					break;
				}
			}

			if (index >= 0)
			{
				this->users[index].username = params.username;
				this->users[index].password = params.password;
				this->users[index].alias = params.alias;
			}
			else
			{
				throw error::not_found("Username '", params.username, "' does not exists");
			}

			return D(_id = index);
		};
	}
	auto GetUser()
	{
		return [this](auto params, user_orm& orm, mysql_connection& db)
		{
			for (int i = 0; i < this->users.size(); ++i)
			{
				if (this->users[i].username == params.username)
				{
					return D(_username = this->users[i].username, _alias = this->users[i].alias);
				}
			}
			throw error::not_found("Username '", params.username, "' does not exists");
		};
	}
#endif
};