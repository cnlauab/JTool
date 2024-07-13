#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Data.h"

class Table
{
public:
	int id;
	std::string name;
	std::unordered_map<std::string, std::string> fieldToType;
	std::vector<Data> data;

private:
	int size;
};

