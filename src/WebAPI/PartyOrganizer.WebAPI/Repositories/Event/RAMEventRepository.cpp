#include "RAMEventRepository.h"

int RAMEventRepository::CreateEvent(std::string& roomName, std::string& eventName, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers)
{
	auto key = std::make_pair(roomName, eventName);
	if (this->events.find(key) != this->events.end())
		return ID_ALREADY_EXISTS;

	std::shared_ptr<Event> event = std::make_shared<Event>(
		Event(eventName, dateStarts, dateEnds, dateCreated, minUsers, maxUsers));

	this->events[key] = event;

	return 0;
}

bool RAMEventRepository::DeleteEvent(std::string& roomName, std::string& eventName)
{
	auto key = std::make_pair(roomName, eventName);
	if (this->events.find(key) == this->events.end())
		return false;

	this->events.erase(key);
	return true;
}

int RAMEventRepository::EditEvent(std::string& roomName, std::string& eventName, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers)
{
	auto key = std::make_pair(roomName, eventName);
	if (this->events.find(key) == this->events.end())
		return -1;

	this->events[key] = std::make_shared<Event>(Event(eventName, dateStarts, dateEnds, dateCreated, minUsers, maxUsers));
}

std::shared_ptr<Event> RAMEventRepository::GetEvent(std::string& roomName, std::string& eventName)
{
	auto key = std::make_pair(roomName, eventName);
	if (this->events.find(key) == this->events.end())
		return nullptr;

	return this->events[key];
}