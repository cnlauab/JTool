#include "Table.h"

Table::Table() {
	id = 0;
	name = "Empty";
}

Table::Table(Json::Value& root) {
	if (root.isMember("id") && root["id"].isInt()) id = root["id"].asInt();
	if (root.isMember("name") && root["name"].isString()) name = root["name"].asString();
	if (root.isMember("schema") && root["schema"].isArray()) {
		int s = root["schema"].size();
		for (int i = 0; i < s; i++) {
			std::string key = root["schema"][i]["name"].asString();
			std::string value = root["schema"][i]["type"].asString();
			schema[key] = value;
		}
	}
	if (root.isMember("data") && root["data"].isArray()) {
		int s = root["data"].size();
		for (int i = 0; i < s; i++) {
			data.push_back(std::unordered_map<std::string, std::string>());
			for (auto field : schema) {
				data[i][field.first] = root["data"][i][field.first].asString();
			}
		}
	}
}

std::string Table::toString() {
	std::string result = "";

	result += "Id: " + std::to_string(id) + "\n";
	result += "Name: " + name + "\n";
	result += "\nSchema: \n";
	for (auto pair : schema) {
		result += pair.first + ", " + pair.second + "\n";
	}
	result += "\nData: \n";
	for (auto row : data) {
		for (auto column : row) {
			result += column.first + ": " + column.second + "\n";
		}
		result += "\n";
	}
	return result;
}