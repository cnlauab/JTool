#include "Field.h"

Field::Field()
{
	name = "Undefined";
	type = "string";
}

Field::Field(const Field& other)
{
	name = other.name;
	type = other.type;
}

Field::Field(Field&& other) noexcept
{
	name = other.name;
	type = other.type;
}

Field& Field::operator=(const Field& other)
{
	if (this != &other) {
		name = other.name;
		type = other.type;
	}
	return *this;
}

Field& Field::operator=(Field&& other) noexcept
{
	if (this != &other) {
		name = other.name;
		type = other.type;
	}
	return *this;
}

Field::~Field()
{
}

bool Field::ValidType(std::string type)
{
	return type == "int" || type == "bool" || type == "string" || type == "float" || type == "bitmap" || type == "pointer";
}
