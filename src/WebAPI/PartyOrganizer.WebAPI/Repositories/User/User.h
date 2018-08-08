#pragma once
#include <string>

class User {
public: 
	User(std::string username, std::string password, std::string alias);
	User();

	const std::string getUsername() const;
	const std::string getPassword() const;
	const std::string getAlias() const;

	void setUsername(std::string username);
	void setPassword(std::string password);
	void setAlias(std::string alias);
private:
	std::string username;
	std::string password;
	std::string alias;
};
