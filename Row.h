#pragma once
#include <string>
#include <vector>
#include <map>

class Row
{
public:
	std::map<std::string, std::string> data;

	Row();
	Row(std::map<std::string, std::string> newData);
	Row(const Row& other);

	bool FieldExist(std::string key);
	void AddField(std::string newKey);
	void AddField(std::string newKey, std::string newValue);
	void DeleteField(std::string key);

	std::string GetCell(std::string key);
	std::map<std::string, std::string> GetRow();
	void EditCell(std::string key, std::string value);

};

