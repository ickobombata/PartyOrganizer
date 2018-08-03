#pragma once

#include "IUserRepository.h"
#include <unordered_map>

class RAMUserRepository : public IUserRepository
{
public:
	virtual ~RAMUserRepository()
	{   }

public:
	virtual int CreateUser(const std::string& username, const std::string& password, const std::string& alias) override;
	virtual bool DeleteUser(const std::string& username) override;
	virtual int EditUser(const std::string& username, const std::string& password, const std::string& alias) override;
	virtual std::shared_ptr<User> GetUser(const std::string& username) override;

private:
	std::unordered_map<std::string, std::shared_ptr<User>> users;
	std::unordered_map<std::string, Multimedia> multimedia;
	std::unordered_map<std::string, Task> tasks;
	std::unordered_map<std::string, Message> messages;
	std::unordered_map<std::string, Chat> chats;
	std::unordered_map<std::string, Status> statuses;
	std::unordered_map<std::string, Event> events;
};
