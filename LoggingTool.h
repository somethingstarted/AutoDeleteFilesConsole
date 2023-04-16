#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>

#include "Formatting.h"




class OutputType
{
public:
			//main ones
	static constexpr int16 _logERROR = 0b0000000000000001;
	static constexpr int16 _WARNING = 0b0000000000000010;
	static constexpr int16 _INFO = 0b0000000000000100;
	static constexpr int16 DEBUG = 0b0000000000001000;
	static constexpr int16 MISC = 0b0000000000010000; // default

	static constexpr int16 VERBOSE = 0b0000000000100000;
	static constexpr int16 CRITICAL = 0b0000000001000000;
	static constexpr int16 PERFORMANCE = 0b0000000010000000;
	static constexpr int16 SECURITY = 0b0000000100000000;
	static constexpr int16 AUDIT = 0b0000001000000000;
	static constexpr int16 METRICS = 0b0000010000000000;
	static constexpr int16 MAINTENANCE = 0b0000100000000000;
	static constexpr int16 SYS_INFO = 0b0001000000000000;

	static constexpr int16 SkipLine = 0b0010000000000000; //only do once when starting the program again

	static constexpr int16 RESET_LOG = 0b1111'1111'1111'1111;
	static constexpr int16 CREATE_USE_NEW_LOG_FILE = 0b0000000000000000;


};

class LoggingTool
{
public:
	void AppendToLog(std::string LoggingString = "", int16 LogOutputType = OutputType::MISC);
	std::string CheckOutputType(int16 LogOutputType);
};