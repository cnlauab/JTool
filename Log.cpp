#include "Log.h"


std::string Log::AppendLog(std::string message) {
	line++;
	std::string result = "[Line " + std::to_string(line) + "] " + message + "\n";
	log += result;

	return log;
}
std::string Log::PrintLog() {
	return log;
}