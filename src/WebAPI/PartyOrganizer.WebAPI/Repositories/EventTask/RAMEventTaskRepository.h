#pragma once

#include "IEventTaskRepository.h"
#include <unordered_map>

class RAMEventTaskRepository : public IEventTaskRepository
{
public:
	virtual ~RAMEventTaskRepository()
	{   }

public:
	virtual int CreateEventTask(std::string& eventName, std::string& taskName, std::string &description, int complexity, int state) override;
	virtual bool DeleteEventTask(std::string& eventName, std::string& taskName) override;
	virtual int EditEventTask(std::string& eventName, std::string& taskName, std::string &description, int complexity, int state) override;
	virtual std::shared_ptr<EventTask> GetEventTask(std::string& eventName, std::string& taskName) override;

private:
	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2> &p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			// Mainly for demonstration purposes, i.e. works but is overly simple
			// In the real world, use sth. like boost.hash_combine
			return h1 ^ h2;
		}
	};

	std::unordered_map< std::pair<std::string, std::string>, std::shared_ptr<EventTask>, pair_hash> events;
};
