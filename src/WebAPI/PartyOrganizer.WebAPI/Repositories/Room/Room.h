#pragma once
#include <string>

class Room {
public: 
	Room(std::string name);
	Room();

	const std::string getName() const;

	void setName(std::string name);
private:
	std::string name;
};
