#pragma once

#include "LoggingService.hpp"
#include "ConfigurationService.h"
#include "ServiceProvider.hpp"
#include "../Repositories/User/RAMUserRepository.h"
#include "../Repositories/Room/RAMRoomRepository.h"
#include "../Repositories/Event/RAMEventRepository.h"
#include "../Repositories/EventTask/RAMEventTaskRepository.h"
#include <unordered_map>
#include <string>

using namespace sl; // Silicon namespace
using namespace s; // symbols namespace

class DatabaseService
{
private: 
	std::string DEFAULT_DATE;
public:
	DatabaseService() : users(nullptr), rooms(nullptr), events(nullptr), eventTasks(nullptr)
	{
		DEFAULT_DATE = "1995/03/25";
		logger = ServiceProvider::Instance().Resolve<LoggingService>();
		configuration = ServiceProvider::Instance().Resolve<ConfigurationService>();

		std::string debugDatabase = configuration->Get("Database", "Debug");
		if (debugDatabase == "true")
		{
			users = std::make_unique<RAMUserRepository>();
			rooms = std::make_unique<RAMRoomRepository>();
			events = std::make_unique<RAMEventRepository>();
			eventTasks = std::make_unique<RAMEventTaskRepository>();
		}
	}
	
public:
	auto CreateUser()
	{
		return [this](auto params) 
		{
			int id = users->CreateUser(params.username, params.password, params.alias);
			if (id == IUserRepository::ID_NOT_FOUND) 
			{
				return D(_status = "fail");
			}

			logger->Info("Created User with id %d", id);

			return D(_status = "ok");
		};
	}

	auto DeleteUser()
	{
		return [this](auto params)
		{
			bool deleted = users->DeleteUser(params.username);
			if (!deleted) 
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}
	auto EditUser()
	{
		return [this](auto params)
		{
			int id = users->EditUser(params.username, params.password, params.alias);
			if (id == IUserRepository::ID_NOT_FOUND) 
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}
	auto GetUser()
	{
		return [this](auto params)
		{	
			std::shared_ptr<User> user = users->GetUser(params.username);
			if (!user) 
			{
				return D(_status = "fail", _username = params.username, _alias = std::string(""));
			}

			return D(_status = "ok", _username = params.username, _alias = user->getAlias());
		};
	}	
	
	auto CreateRoom()
	{
		return [this](auto params)
		{
			bool created = rooms->CreateRoom(params.name);
			if (!created)
			{
				return D(_status = "fail");
			}

			logger->Info("Created Room with name %s", params.name);

			return D(_status = "ok");
		};
	}
	auto DeleteRoom()
	{
		return [this](auto params)
		{
			bool deleted = rooms->DeleteRoom(params.name);
			if (!deleted)
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}

	auto CreateEvent()
	{
		return [this](auto params)
		{
			int id = events->CreateEvent(params.roomName, params.eventName, params.dateStarts, params.dateEnds, DEFAULT_DATE, params.minUsers, params.maxUsers);
			if (id == IEventRepository::ID_NOT_FOUND)
			{
				return D(_status = "fail");
			}

			logger->Info("Created Event with id %d", id);

			return D(_status = "ok");
		};
	}

	auto DeleteEvent()
	{
		return [this](auto params)
		{
			bool deleted = events->DeleteEvent(params.roomName, params.eventName);
			if (!deleted)
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}

	auto EditEvent()
	{
		return [this](auto params)
		{
			int id = events->EditEvent(params.roomName, params.eventName, params.dateStarts, params.dateEnds, DEFAULT_DATE, params.minUsers, params.maxUsers);
			if (id == IEventRepository::ID_NOT_FOUND)
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}

	auto GetEvent()
	{
		return [this](auto params)
		{
			std::shared_ptr<Event> event = events->GetEvent(params.roomName, params.eventName);
			if (!event)
			{
				return D(_status = "fail", _name = std::string(""), _dateStarts = std::string(""), _dateEnds = std::string(""), _dateCreated = std::string(""), _minUsers = int(-1), _maxUsers = int(-1));
			}

			return D(_status = "ok", _name = event.get()->getName(), _dateStarts = event.get()->getDateStarts(), _dateEnds = event.get()->getDateEnds(), _dateCreated = event.get()->getDateCreated(), _minUsers = event.get()->getMinUsers(), _maxUsers = event.get()->getMaxUsers());
		};
	}

	auto CreateEventTask()
	{
		return [this](auto params)
		{
			int id = eventTasks->CreateEventTask(params.eventName, params.eventTaskName, params.description, params.complexity, params.state);
			if (id == IEventTaskRepository::ID_NOT_FOUND)
			{
				return D(_status = "fail");
			}

			logger->Info("Created EventTask with id %d", id);

			return D(_status = "ok");
		};
	}

	auto DeleteEventTask()
	{
		return [this](auto params)
		{
			bool deleted = eventTasks->DeleteEventTask(params.eventName, params.eventTaskName);
			if (!deleted)
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}

	auto EditEventTask()
	{
		return [this](auto params)
		{
			int id = eventTasks->EditEventTask(params.eventName, params.eventTaskName, params.description, params.complexity, params.state);
			if (id == IEventTaskRepository::ID_NOT_FOUND)
			{
				return D(_status = "fail");
			}

			return D(_status = "ok");
		};
	}

	auto GetEventTask()
	{
		return [this](auto params)
		{
			std::shared_ptr<EventTask> eventTask = eventTasks->GetEventTask(params.eventName, params.eventTaskName);
			if (!eventTask)
			{
				return D(_status = "fail", _name = params.eventTaskName, _description = std::string(""), _complexity = int(-1), _state = int(-1));
			}
			//std::string name, std::string description, int complexity, int state
			return D(_status = "ok", _name = eventTask.get()->getName(), _description = eventTask.get()->getDescription(), _complexity = eventTask.get()->getComplexity(), _state = eventTask.get()->getState());
		};
	}

private:
	std::shared_ptr<LoggingService> logger;
	std::shared_ptr<ConfigurationService> configuration;
	std::unique_ptr<IUserRepository> users;
	std::unique_ptr<IRoomRepository> rooms;
	std::unique_ptr<IEventRepository> events;
	std::unique_ptr<IEventTaskRepository> eventTasks;
}; 
