#pragma once
#include <string>

class Event {
public: 
	Event(std::string& name, std::string &dateStarts, std::string &dateEnds, std::string &dateCreated, int minUsers, int maxUsers);

	const std::string getName() const;
	const std::string getDateStarts() const;
	const std::string getDateEnds() const;
	const std::string getDateCreated() const;
	int getMinUsers() const;
	int getMaxUsers() const;

	void setName(std::string &name);
	void setDateStarts(std::string &dateStarts);
	void setDateEnds(std::string &dateEnds);
	void setDateCreated(std::string &dateCreated);
	void setMinUsers(int minUsers);
	void setMaxUsers(int maxUsers);
private:
	std::string name;
	std::string dateStarts;
	std::string dateEnds;
	std::string dateCreated;
	int minUsers;
	int maxUsers;
};
