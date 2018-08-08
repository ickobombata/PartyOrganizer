#include "User.h"

User::User(std::string username, std::string password, std::string alias)
{
	setUsername(username);
	setPassword(password);
	setAlias(alias);
}

User::User()
{
	setUsername("");
	setPassword("");
	setAlias("");
}
const std::string User::getUsername() const
{
	return this->username;
}

const std::string User::getPassword() const
{
	return this->password;
}

const std::string User::getAlias() const
{
	return this->alias;
}

void User::setUsername(std::string username)
{
	this->username = username;
}

void User::setPassword(std::string password)
{
	this->password = password;
}

void User::setAlias(std::string alias)
{
	this->alias = alias;
}