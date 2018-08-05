#include "Event.h"

Event::Event(std::string& name, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers)
{
	setName(name);
	setDateStarts(dateStarts);
	setDateEnds(dateEnds);
	setDateCreated(dateCreated);
	setMinUsers(minUsers);
	setMaxUsers(maxUsers);
}

const std::string Event::getName() const
{
	return this->name;
}

const std::string Event::getDateStarts() const
{
	return this->dateStarts;
}

const std::string Event::getDateEnds() const
{
	return this->dateEnds;
}

const std::string Event::getDateCreated() const
{
	return this->dateCreated;
}

int Event::getMinUsers() const
{
	return this->minUsers;
}

int Event::getMaxUsers() const
{
	return this->maxUsers;
}

void Event::setName(std::string &name)
{
	this->name = name;
}

void Event::setDateStarts(std::string &dateStarts)
{
	this->dateStarts = dateStarts;
}

void Event::setDateEnds(std::string &dateEnds)
{
	this->dateEnds = dateEnds;
}

void Event::setDateCreated(std::string &dateCreated)
{
	//if (validateDate -> this will be remvoed when we introduce Date clsas or reuse some Date shit. not important for now)
	this->dateCreated = dateCreated;
}

void Event::setMinUsers(int minUsers)
{
	if (minUsers > 0)
		this->minUsers = minUsers;
	else
		this->minUsers = 0;
}

void Event::setMaxUsers(int maxUsers)
{
	if (maxUsers > 0)
		this->maxUsers = maxUsers;
	else
		this->maxUsers = 0;
}