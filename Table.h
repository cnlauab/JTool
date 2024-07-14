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

	Table(int _id, std::string _name) : id(_id), name(_name) {}

private:
	int size;
};

