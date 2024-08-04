#pragma once

#include <string>
#include <chrono>
#include <ctime>

class Log
{
public:
	int line = 0;
	std::string log = "Log starts here...\n";

	std::string AppendLog(std::string messag);
	std::string PrintLog();
};

