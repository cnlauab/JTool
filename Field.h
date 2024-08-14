#pragma once
#include <string>

class Field
{
public:
	std::string name;
	std::string type;

	Field();
	Field(std::string name, std::string type) : name(name), type(type){}
	
	static bool ValidType(std::string type);
};

