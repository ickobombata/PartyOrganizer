#pragma once

#include "IEventRepository.h"
#include <unordered_map>

class RAMEventRepository : public IEventRepository
{
public:
	virtual ~RAMEventRepository()
	{   }

public:
	virtual int CreateEvent(std::string& name, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers) override;
	virtual bool DeleteEvent(std::string& name) override;
	virtual int EditEvent(std::string& name, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers) override;
	virtual std::shared_ptr<Event> GetEvent(std::string& username) override;

private:
	std::unordered_map<std::string, std::shared_ptr<Event>> events;
};
