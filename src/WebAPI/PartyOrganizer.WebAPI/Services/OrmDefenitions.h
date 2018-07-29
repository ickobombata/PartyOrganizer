#pragma once
#include "../symbols.hh"

using namespace s; // Symbols namespace

typedef decltype(D(_username(_primary_key) = std::string(),
	_password = std::string(),
	_alias = std::string()))
	User;