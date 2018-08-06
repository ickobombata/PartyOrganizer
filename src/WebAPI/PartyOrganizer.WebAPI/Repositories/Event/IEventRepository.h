#pragma once

//#include "../../Services/OrmDefinitions.h"
#include <memory>
#include "Event.h"

class IEventRepository
{
public:
	static const int ID_ALREADY_EXISTS = -2;

public:
	virtual ~IEventRepository()
	{   }

public:
	virtual int CreateEvent(std::string& roomName, std::string& eventName, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers) = 0;
	virtual bool DeleteEvent(std::string& roomName, std::string& eventName) = 0;
	virtual int EditEvent(std::string& roomName, std::string& eventName, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers) = 0;
	virtual std::shared_ptr<Event> GetEvent(std::string& roomName, std::string& eventName) = 0;
};
