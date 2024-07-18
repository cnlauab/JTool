#include "Table.h"

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

	}
}

std::string Table::toString() {
	std::string result = "";

	result += "Id: " + std::to_string(id) + "\n";
	result += "Name: " + name + "\n";
	result += "Schema: \n";
	for (auto pair : schema) {
		result += pair.first + ", " + pair.second + "\n";
	}

	return result;
}