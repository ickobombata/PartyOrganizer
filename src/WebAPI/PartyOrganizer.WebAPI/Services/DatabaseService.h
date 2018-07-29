#pragma once

#include "OrmDefenitions.h"

using namespace sl; // Silicon namespace
using namespace s;

typedef mysql_orm<user> user_orm;


class DatabaseService {
private:

public:

	auto create_user(std::string username, std::string password, std::string alias) {
		return [](auto params, user_orm& orm, mysql_connection& db) {
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
		};
	}
		
	/*
	http_verb<http_get> delete_user = GET / _delete_user * get_parameters(_username = std::string())
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
	};

	http_verb<http_get> update_user = GET / _update_user * get_parameters(_username = std::string(),
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
	};*/
};