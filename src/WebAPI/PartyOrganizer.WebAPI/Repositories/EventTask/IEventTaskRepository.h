#pragma once

//#include "../../Services/OrmDefinitions.h"
#include <memory>
#include "EventTask.h"

class IEventTaskRepository
{
public:
	static const int ID_NOT_FOUND = -1;
	static const int ID_ALREADY_EXISTS = -2;

public:
	virtual ~IEventTaskRepository()
	{   }

public:
	virtual int CreateEventTask(std::string& eventName, std::string& taskName, std::string &description, int complexity, int state) = 0;
	virtual bool DeleteEventTask(std::string& eventName, std::string& taskName) = 0;
	virtual int EditEventTask(std::string& eventName, std::string& taskName, std::string &description, int complexity, int state) = 0;
	virtual std::shared_ptr<EventTask> GetEventTask(std::string& eventName, std::string& taskName) = 0;
};
