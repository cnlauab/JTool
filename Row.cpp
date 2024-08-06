#include "Row.h"

Row::Row() {}
Row::Row(std::map<std::string, std::string> newData) {
	for (auto cell : newData) {
		data[cell.first] = cell.second;
	}
}
Row::Row(const Row& other) {
	for (auto cell : other.data) {
		data[cell.first] = cell.second;
	}
}

bool Row::FieldExist(std::string key) {
	return data.find(key) != data.end();
}
void Row::AddField(std::string newKey) {
	if (!FieldExist(newKey)) {
		data[newKey] = "";
	}
}
void Row::AddField(std::string newKey, std::string newValue) {
	if (!FieldExist(newKey)) {
		data[newKey] = newValue;
	}
}
void Row::DeleteField(std::string key) {
	if (FieldExist(key)) {
		data.erase(key);
	}
}

std::string Row::GetCell(std::string key) {
	if (FieldExist(key)) {
		return data[key];
	}
	else {
		return "";
	}
}
std::map<std::string, std::string> Row::GetRow() {
	return data;
}
void Row::EditCell(std::string key, std::string value) {
	if (FieldExist(key)) {
		data[key] = value;
	}
}