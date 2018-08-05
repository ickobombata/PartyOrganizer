#include "RAMEventRepository.h"

int RAMEventRepository::CreateEvent(std::string& name, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers)
{
	if (this->events.find(name) != this->events.end())
		return ID_ALREADY_EXISTS;

	std::shared_ptr<Event> event = std::make_shared<Event>(
		Event(name, dateStarts, dateEnds, dateCreated, minUsers, maxUsers));

	this->events[name] = event;

	return 0;
}

bool RAMEventRepository::DeleteEvent(std::string& name)
{
	if (this->events.find(name) == this->events.end())
		return false;

	this->events.erase(name);
	return true;
}

int RAMEventRepository::EditEvent(std::string& name, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers)
{
	if (this->events.find(name) == this->events.end())
		return -1;

	this->events[name] = std::make_shared<Event>(Event(name, dateStarts, dateEnds, dateCreated, minUsers, maxUsers));
}

std::shared_ptr<Event> RAMEventRepository::GetEvent(std::string& username)
{
	if (this->events.find(username) == this->events.end())
		return nullptr;

	return this->events[username];
}