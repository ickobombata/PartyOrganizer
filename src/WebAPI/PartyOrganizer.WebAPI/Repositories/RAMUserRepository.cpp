#include "RAMUserRepository.h"

int RAMUserRepository::CreateUser(const std::string& username, const std::string& password, const std::string& alias)
{
	if (this->users.find(username) == this->users.end())
		return ID_NOT_FOUND;

	std::shared_ptr<User> user = std::make_shared<User>();
	user->username = username;
	user->password = password;
	user->alias = alias;

	this->users[username] = user;

	//logger->Info("Created User with id %d", this->users.size());
	return 0;
}

bool RAMUserRepository::DeleteUser(const std::string& username)
{
	if (this->users.find(username) == this->users.end())
		return false;

	this->users.erase(username);
	return true;
}

int RAMUserRepository::EditUser(const std::string& username, const std::string& password, const std::string& alias)
{
	if (this->users.find(username) == this->users.end())
		return -1;

	this->users[username]->password = password;
	this->users[username]->alias = alias;
}

std::shared_ptr<User> RAMUserRepository::GetUser(const std::string& username)
{
	if (this->users.find(username) == this->users.end())
		return nullptr;

	return this->users[username];
}
