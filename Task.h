#pragma once
#include <string>
#include <vector>

class Task
{
public:
	std::string description;
	bool done;
};

void saveTaskToFile(const std::vector<Task>& tasks, const std::string& fileName);

std::vector<Task> loadTasksFromFile(const std::string& fileName);