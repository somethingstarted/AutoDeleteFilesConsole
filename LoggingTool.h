#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>
//add csv libray, figure out how to install rapidcsv. 
	//well, how to add it to the project as a dependency. 
	//it could be a great way to keep track of files that 
	//are processed through this
	// 
#include "Formatting.h"
//class Formatting;


class WhichClassUsed
{
public:

	//list all local classes used in this project as 32 bit bitwise
	static constexpr uint16 MiscClass_Which = 0b00000000;
	static constexpr uint16 FileSystemWatcher_Which = 0b00000001;
	static constexpr uint16 DirectoryIndexing_Which = 0b00000010;
	static constexpr uint16 FileDeleterFrame_Which = 0b00000100;
	static constexpr uint16 WatcherThread_Which = 0b00001000;
	static constexpr uint16 LoggingTool_Which = 0b00010000;
	static constexpr uint16 Formatting_Which = 0b00100000;
	static constexpr uint16 FileSystemWatcherThread_Which = 0b01000000;
	static constexpr uint16 AutoDeleteFiles_Which = 0b10000000;


	static constexpr uint16 ExternalLibraries = 0b1111111111111111;

};

class OutputType
{
public:
			//main ones
	static constexpr uint16 _logERROR = 0b0000000000000001;
	static constexpr uint16 _WARNING = 0b0000000000000010;
	static constexpr uint16 _INFO = 0b0000000000000100;
	static constexpr uint16 DEBUG = 0b0000000000001000;
	static constexpr uint16 MISC = 0b0000000000010000; // default

	static constexpr uint16 VERBOSE = 0b0000000000100000;
	static constexpr uint16 CRITICAL = 0b0000000001000000;
	static constexpr uint16 PERFORMANCE = 0b0000000010000000;
	static constexpr uint16 SECURITY = 0b0000000100000000;
	static constexpr uint16 AUDIT = 0b0000001000000000;
	static constexpr uint16 METRICS = 0b0000010000000000;
	static constexpr uint16 MAINTENANCE = 0b0000100000000000;
	static constexpr uint16 SYS_INFO = 0b0001000000000000;

	static constexpr uint16 SkipLine = 0b0010000000000000; //only do once when starting the program again

	static constexpr uint16 RESET_LOG = 0b1111'1111'1111'1111;
	static constexpr uint16 CREATE_USE_NEW_LOG_FILE = 0b0000000000000000;


};

class LoggingTool
{
public:
	void AppendToLog(std::string LoggingString = "", uint16 LogOutputType = OutputType::MISC, uint16 WhichClassUsed = WhichClassUsed::MiscClass_Which);
	std::string CheckOutputType(uint16 LogOutputType);

	//char length of longest class name (for formatting class in WhichClassUsed)
	static constexpr int8 MaxClassNameLength = 23;
};

extern LoggingTool* logging_tool;