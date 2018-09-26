#include "RAMUserRepository.h"

int RAMUserRepository::CreateUser(const std::string& username, const std::string& password, const std::string& alias)
{
	if (this->users.find(username) != this->users.end())
		return ID_ALREADY_EXISTS;

	std::shared_ptr<User> user = std::make_shared<User>();
	user->setUsername(username);
	user->setPassword(password);
	user->setAlias(alias);

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
		return ID_NOT_FOUND;

	this->users[username]->setPassword(password);
	this->users[username]->setAlias(alias);
}

std::shared_ptr<User> RAMUserRepository::GetUser(const std::string& username)
{
	if (this->users.find(username) == this->users.end())
		return nullptr;

	return this->users[username];
}
