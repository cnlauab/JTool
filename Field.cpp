#include "Field.h"

bool Field::ValidType(std::string type)
{
	return type == "int" || type == "bool" || type == "string" || type == "float" || type == "bitmap" || type == "pointer";
}
