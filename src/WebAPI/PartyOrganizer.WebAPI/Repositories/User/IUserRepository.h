#pragma once

#include <memory>
#include "User.h"

class IUserRepository
{
public:
	static const int ID_NOT_FOUND = -1;
	static const int ID_ALREADY_EXISTS = -2;
	
public:
	virtual ~IUserRepository()
	{   }

public:
	virtual int CreateUser(const std::string& username, const std::string& password, const std::string& alias) = 0;
	virtual bool DeleteUser(const std::string& username) = 0;
	virtual int EditUser(const std::string& username, const std::string& password, const std::string& alias) = 0;
	virtual std::shared_ptr<User> GetUser(const std::string& username) = 0;
};
