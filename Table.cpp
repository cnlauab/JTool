#include "Table.h"

Table::Table() {
	id = 0;
	name = "Empty";
	size = 0;
}

Table::Table(Json::Value& root) {
	id = 0;
	name = "Empty";
	size = 0;
	if (root.isMember("id") && root["id"].isInt()) id = root["id"].asInt();
	if (root.isMember("name") && root["name"].isString()) name = root["name"].asString();
	if (root.isMember("schema") && root["schema"].isArray()) {
		int s = root["schema"].size();
		for (int i = 0; i < s; i++) {
			std::string key = root["schema"][i]["name"].asString();
			std::string value = root["schema"][i]["type"].asString();
			schema.emplace_back(std::pair( key,value ));
			fieldIndex[key] = i;
		}
	}
	if (root.isMember("data") && root["data"].isArray()) {
		int s = root["data"].size();
		for (int i = 0; i < s; i++) {
			size++;
			std::map<std::string, std::string> row;
			for (auto field : schema) {
				std::string key = field.first;
				std::string value = root["data"][i][field.first].asString();
				row.insert({ key, value});
			}
			AddRow(row);
		}
	}
}

bool Table::AddField(std::string name, std::string type)
{
	bool result = fieldIndex.find(name) == fieldIndex.end();
	if (result) {
		schema.emplace_back(std::pair(name, type));
		fieldIndex[name] = schema.size() - 1;
	}
	return result;
}

bool Table::DeleteField(std::string name)
{
	bool result = fieldIndex.find(name) != fieldIndex.end();
	if (result) {
		//fieldIndex
		int i = fieldIndex[name];
		fieldIndex.erase(name);
		for (auto& field : fieldIndex) {
			if (field.second > i) {
				field.second--;
			}
		}
		//schema
		for (int j = i; j <= schema.size() - 2; j++) {
			schema[j].first = schema[j + 1].first;
			schema[j].second = schema[j + 1].second;
		}
		schema.pop_back();
		//data
		for (auto& row : data) {
			row.second.DeleteField(name);
		}
	}
	return result;
}

bool Table::EditField(std::string name, std::string type)
{
	bool result = fieldIndex.find(name) != fieldIndex.end();
	if (result) {
		int i = fieldIndex[name];
		schema[i].second = type;
	}
	return result;
}

bool Table::AddRow(std::map<std::string, std::string>& entry)
{
	int i = data.size();
	data.insert({ i, Row(entry) });
	size++;
	return true;
}

bool Table::DeleteRow(int index)
{
	bool result = index < data.size();
	if (result) {
		for(int i = index; i < data.size() - 1; i++){
			data[i] = data[i + 1];
		}
		data.erase(data.size() - 1);
		size--;
	}
	return result;
}

void Table::EditRow(int index, std::pair<std::string, std::string> cell)
{
	data[index].EditCell(cell.first, cell.second);
}

std::string Table::toString() {
	std::string result = "";

	result += "Id: " + std::to_string(id) + "\n";
	result += "Name: " + name + "\n";
	result += "\nSchema: \n";
	for (auto pair : schema) {
		result += pair.first + ", " + pair.second + "\n";
	}

	result += "\nField Index: \n";
	for (auto pair : fieldIndex) {
		result += pair.first + ", " + std::to_string(pair.second) + "\n";
	}
	result += "\nData: \n";
	for (auto row : data) {
		for (auto column : row.second.data) {
			result += column.first + ": " + column.second + "\n";
		}
		result += "\n";
	}
	return result;
}

Json::Value Table::toJson()
{
	Json::Value root;
	root["id"] = id;
	root["name"] = name;
	for (int i = 0; i < schema.size(); i++) {
		root["schema"][i]["name"] = schema[i].first;
		root["schema"][i]["type"] = schema[i].second;
	}
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < schema.size(); j++) {
			std::string key = schema[j].first;
			root["data"][i][key] = data[i].GetCell(key);
		}
	}
	return root;
}
