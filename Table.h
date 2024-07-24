#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <json/json.h>

class Table
{
public:
	int id;
	std::string name;
	std::map<std::string, std::string> schema;
	std::unordered_map<std::string, int> columnIndex;
	std::vector<std::unordered_map<std::string, std::string>> data;

	Table();
	Table(int _id, std::string _name) : id(_id), name(_name) {}
	Table(Json::Value& root);

	void AddField(std::string& name, std::string& type);
	void DeleteField(std::string& name);
	void AddRow(std::unordered_map<std::string, std::string>& entry);
	void DeleteRow(int index);
	void InsertRow(int index);

	std::string toString();
	std::string toJson();
private:
	int size;
};

