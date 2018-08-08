#include "RAMEventTaskRepository.h"

int RAMEventTaskRepository::CreateEventTask(std::string& eventName, std::string& taskName, std::string &description, int complexity, int state)
{
	auto key = std::make_pair(eventName, taskName);
	if (this->events.find(key) != this->events.end())
		return ID_ALREADY_EXISTS;

	std::shared_ptr<EventTask> event = std::make_shared<EventTask>(
		EventTask(taskName, description, complexity, state));

	this->events[key] = event;

	return 0;
}

bool RAMEventTaskRepository::DeleteEventTask(std::string& roomName, std::string& eventName)
{
	auto key = std::make_pair(roomName, eventName);
	if (this->events.find(key) == this->events.end())
		return false;

	this->events.erase(key);
	return true;
}

int RAMEventTaskRepository::EditEventTask(std::string& eventName, std::string& taskName, std::string &description, int complexity, int state)
{
	auto key = std::make_pair(eventName, taskName);
	if (this->events.find(key) == this->events.end())
		return -1;

	this->events[key] = std::make_shared<EventTask>(EventTask(taskName, description, complexity, state));
}

std::shared_ptr<EventTask> RAMEventTaskRepository::GetEventTask(std::string& roomName, std::string& eventName)
{
	auto key = std::make_pair(roomName, eventName);
	if (this->events.find(key) == this->events.end())
		return nullptr;

	return this->events[key];
}