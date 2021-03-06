#pragma once

#include "IUserRepository.h"


#ifdef USE_DB

class UserRepository : public IUserRepository
{

};

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