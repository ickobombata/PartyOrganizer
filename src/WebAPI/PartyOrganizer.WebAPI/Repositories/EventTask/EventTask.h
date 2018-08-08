#pragma once
#include <string>

class EventTask {
public: 
	EventTask(std::string name, std::string description, int complexity, int state);
	EventTask();
	const std::string getName() const;
	const std::string getDescription() const;
	const int getComplexity() const;
	const int getState() const;

	void setName(std::string name);
	void setDescription(std::string description);
	void setComplexity(int complexity);
	void setState(int state);
private:
	std::string name;
	std::string description;
	int complexity;
	int state;
};
