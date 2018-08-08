#include "EventTask.h"

EventTask::EventTask(std::string name, std::string description, int complexity, int state)
{
	setName(name);
	setDescription(description);
	setComplexity(complexity);
	setState(state);
}

EventTask::EventTask()
{
	setName("");
	setDescription("");
	setComplexity(-1);
	setState(-1);
}

const std::string EventTask::getName() const
{
	return this->name;
}

const std::string EventTask::getDescription() const
{
	return this->description;
}

const int EventTask::getComplexity() const
{
	return this->complexity;
}

const int EventTask::getState() const
{
	return this->state;
}

void EventTask::setName(std::string name)
{
	this->name = name;
}

void EventTask::setDescription(std::string description)
{
	this->description = description;
}

void EventTask::setComplexity(int complexity)
{
	this->complexity = complexity;
}

void EventTask::setState(int dateCreated)
{
	this->state = state;
}