#include "Room.h"

Room::Room(std::string username)
{
	setName(username);
}

Room::Room()
{
	setName("");
}
const std::string Room::getName() const
{
	return this->name;
}

void Room::setName(std::string name)
{
	this->name = name;
}
