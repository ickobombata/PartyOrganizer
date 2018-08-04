#include "RAMRoomRepository.h"

int RAMRoomRepository::CreateRoom(const std::string& name)
{
	if (this->rooms.find(name) == this->rooms.end())
		return ID_NOT_FOUND;

	std::shared_ptr<Room> room = std::make_shared<Room>();
	room->name = name;

	this->rooms[name] = room;

	return 0;
}

bool RAMRoomRepository::DeleteRoom(const std::string& name)
{
	if (this->rooms.find(name) == this->rooms.end())
		return false;

	this->rooms.erase(name);
	return true;
}