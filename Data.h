#pragma once
class Data
{
public:
	int id;
	std::unordered_map<std::string, std::string> fieldToValue;

	std::vector<int> nextIds;
	int prevIds;
};

