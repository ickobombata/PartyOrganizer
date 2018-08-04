#pragma once

#include "../../Services/OrmDefinitions.h"
#include <memory>

class IRoomRepository
{
public:
	static const int ID_NOT_FOUND = -1;

public:
	virtual ~IRoomRepository()
	{   }

public:
	virtual int CreateRoom(const std::string& name) = 0;
	virtual bool DeleteRoom(const std::string& name) = 0;
};
