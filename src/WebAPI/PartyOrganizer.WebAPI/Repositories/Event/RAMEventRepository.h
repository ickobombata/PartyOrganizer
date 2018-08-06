#pragma once

#include "IEventRepository.h"
#include <unordered_map>

class RAMEventRepository : public IEventRepository
{
public:
	virtual ~RAMEventRepository()
	{   }

public:
	virtual int CreateEvent(std::string& roomName, std::string& eventName, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers) override;
	virtual bool DeleteEvent(std::string& roomName, std::string& eventName) override;
	virtual int EditEvent(std::string& roomName, std::string& eventName, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers) override;
	virtual std::shared_ptr<Event> GetEvent(std::string& roomName, std::string& eventName) override;

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

	std::unordered_map< std::pair<std::string, std::string>, std::shared_ptr<Event>, pair_hash> events;
};
