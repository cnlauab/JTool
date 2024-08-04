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
			data.push_back(Row());
			for (auto field : schema) {
				std::string key = field.first;
				std::string value = root["data"][i][field.first].asString();
				data[i].AddField(key, value);
			}
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
			row.DeleteField(name);
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
		for (auto column : row.data) {
			result += column.first + ": " + column.second + "\n";
		}
		result += "\n";
	}
	return result;
}