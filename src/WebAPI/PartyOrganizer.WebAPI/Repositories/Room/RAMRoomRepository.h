#pragma once

#include "IRoomRepository.h"
#include <unordered_map>

class RAMRoomRepository : public IRoomRepository
{
	struct Room {
		std::string name;
	};
public:
	virtual ~RAMRoomRepository()
	{   }

public:
	virtual int CreateRoom(const std::string& name) override;
	virtual bool DeleteRoom(const std::string& name) override;

private:
	std::unordered_map<std::string, std::shared_ptr<Room>> rooms;
};
