#pragma once
#include <string>

class Field
{
public:
	std::string name;
	std::string type;

	Field();
	Field(std::string name, std::string type) : name(name), type(type){}
	Field(const Field& other);
	Field(Field&& other) noexcept;
	Field& operator=(const Field& other);
	Field& operator=(Field&& other) noexcept;
	~Field();
	static bool ValidType(std::string type);
};

