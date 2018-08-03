#pragma once

#include "../Services/OrmDefenitions.h"

class IUserRepository
{
public:
	static const int ID_NOT_FOUND = -1;
	
public:
	virtual ~IUserRepository() = 0;

public:
	virtual int CreateUser(const std::string& username, const std::string& password, const std::string& alias) = 0;
	virtual bool DeleteUser(const std::string& username) = 0;
	virtual int EditUser(const std::string& username, const std::string& password, const std::string& alias) = 0;
	virtual User* GetUser(const std::string& username) = 0;
};
