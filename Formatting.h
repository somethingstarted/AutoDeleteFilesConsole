#pragma once
#include <bitset>
#include <filesystem>
#include <chrono>

//#include "FileDeleterFrame.h"

namespace fs = std::filesystem;
using chrono_ftt = std::filesystem::file_time_type;
using chrono_duration = std::chrono::system_clock::duration;
using chrono_sys_clock = std::chrono::system_clock;

const int SizeOfBits = 16;

typedef   std::int64_t int64;
typedef   std::int32_t int32;
typedef   std::int16_t int16;
typedef   std::int8_t int8;
typedef	  std::uint8_t uint8;
typedef   std::uint16_t uint16;
typedef   std::uint32_t uint32;
typedef   std::uint64_t uint64;

class Formatting
{
	
	public:
		std::string IntergerWithCommas(int64);
		//std::string IntergerWithCommas(int64);
		std::stringstream GetFileAge(chrono_ftt);
		std::string BitsWithSpaces(std::bitset<SizeOfBits>);
		std::wstring StringToWstring(const std::string&);
};

//////////////////////////////////////////////////
// 
// figure out what compiler you're using
// just for help with debugging. 
// i plan on open sourcing this and shit someday
// 
//////////////////////////////////////////////


// Define MSVC_VERSION macro based on _MSC_VER
// Define COMPILER_VERSION macro based on the compiler used
#if _MSC_VER == 1500
#define COMPILER_VERSION "MSVC++ 9.0"
#elif _MSC_VER == 1400
#define COMPILER_VERSION "MSVC++ 8.0"
#elif _MSC_VER == 1310
#define COMPILER_VERSION "MSVC++ 7.1"
#elif _MSC_VER == 1300
#define COMPILER_VERSION "MSVC++ 7.0"
#elif _MSC_VER == 1200
#define COMPILER_VERSION "MSVC++ 6.0"
#elif _MSC_VER == 1100
#define COMPILER_VERSION "MSVC++ 5.0"
#elif _MSC_VER >= 1930 && _MSC_VER < 1940
#define COMPILER_VERSION "MSVC++ 2022 (17.x)"
#elif _MSC_VER >= 1920 && _MSC_VER < 1930
#define COMPILER_VERSION "MSVC++ 2019 (16.x)"
#elif __clang__
#define COMPILER_VERSION "Clang " __clang_version__
#else
#define COMPILER_VERSION "Unknown Compiler"
#endif

