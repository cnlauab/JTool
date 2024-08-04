#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <json/json.h>

#include "Row.h"

class Table
{
public:
	int id;
	std::string name;
	std::vector<std::pair<std::string, std::string>> schema;
	std::vector<Row> data;
	std::unordered_map<std::string, int> fieldIndex;

	Table();
	Table(int _id, std::string _name) : id(_id), name(_name) {}
	Table(Json::Value& root);

	bool AddField(std::string name, std::string type);
	bool DeleteField(std::string name);
	bool EditField(std::string name, std::string type);

	void AddRow(std::map<std::string, std::string>& entry);
	void DeleteRow(int index);
	void InsertRow(int index);

	std::string toString();
	std::string toJson();
private:
	int size;
};

