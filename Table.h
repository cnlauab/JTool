#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <json/json.h>

#include "Field.h"
#include "Row.h"

class Table
{
public:
	int id;
	std::string name;
	std::vector<Field> schema;
	std::map<int,Row> data;
	std::unordered_map<std::string, int> fieldIndex;

	Table();
	Table(int _id, std::string _name) : id(_id), name(_name) {}
	Table(Json::Value& root);

	bool AddField(std::string name, std::string type);
	bool DeleteField(std::string name);
	bool EditField(std::string name, std::string type);

	bool AddRow(std::map<std::string, std::string>& entry);
	bool DeleteRow(int index);
	void EditRow(int index, std::pair<std::string, std::string> cell);

	std::string toString();
	Json::Value toJson();
private:
	int size;
};

