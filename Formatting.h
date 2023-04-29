#pragma once
#include <bitset>
#include <filesystem>
#include <chrono>

//#include "LoggingTool.h"
//class LoggingTool;

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

	template <typename T>
	static T GetLengthOf_T(T& a, bool CountDotIfDecimal = {}, bool CountSpacesIfString = {});
};

// add <std::string>
// add <std::vector<std::string>>
// add <std::array<std::string, SizeOfBits>>
// add <int>
// add <int64>
// add <int32>
// add <int16>
// add <int8>
// add <uint64>
// add <uint32>
// add <uint16>
// add <uint8>
// add <std::bitset<SizeOfBits>>
// add <char>
// add<float>
// add<double>
// add<long double>
// add<wchar_t>
// add<char16_t>
// add<char32_t>
// add<std::list<std::string>>
// add<std::deque<std::string>>
// add<std::forward_list<std::string>>
// add<std::set<std::string>>
template <typename T>
inline T Formatting::GetLengthOf_T(T& a, bool CountDotIfDecimal, bool CountSpacesIfString)
{
	T length{};
	
	//count list, deque, forward list, or set: 
	//if constexpr (std::is_same_v<T, std::list<std::string>> || std::is_same_v<T, std::deque<std::string>> || std::is_same_v<T, std::forward_list<std::string>> || std::is_same_v<T, std::set<std::string>>)
	//{
	//	for (auto& i : a)
	//	{
	//		length += i.length();
	//	}
	//}
	//count digits in standard int types, int, long, long long, short:
	if constexpr (std::is_same_v<T, int> || std::is_same_v<T, long> || std::is_same_v<T, long long> || std::is_same_v<T, short>)
	{
		if (a == 0)
			length = 1;
		else
		{
			while (a != 0)
			{
				a /= 10;
				++length;
			}
		}
	}
	//count digits in all common char types: 
	else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>)
	{
		length = 1;
	}
	//count digits in float, double, or long double: 
	else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, long double>)
	{
		std::string s = std::to_string(a);
		length = s.length();
	}
	//do bitset
	else if constexpr (std::is_same_v<T, std::bitset<SizeOfBits>>)
	{
		for (int i = 0; i < SizeOfBits; ++i)
		{
			if (a[i] == 1)
				++length;
		}
	}
	//string	count all characters, even spaces, if CountSpacesIfString is false,return total-spaces
	else if constexpr (std::is_same_v<T, std::string>)
	{
		if (CountSpacesIfString)
			length = a.length();
		else
		{
			for (auto& c : a)
			{
				if (c != ' ')
					++length;
			}
		}
	}
	//get length of all namespace ints (int64 through int8, and uint64 through uint8) 
	else if constexpr (std::is_same_v<T, int64> || std::is_same_v<T, int32> || std::is_same_v<T, int16> || std::is_same_v<T, int8> || std::is_same_v<T, uint64> || std::is_same_v<T, uint32> || std::is_same_v<T, uint16> || std::is_same_v<T, uint8>)
	{
		if (a == 0)
			length = 1;
		else
		{
			while (a != 0)
			{
				a /= 10;
				++length;
			}
		}
	}	
	// vector
	else if constexpr (std::is_same_v<T, std::vector<std::string>>)
		length = a.size();
	//array
	else if constexpr (std::is_same_v<T, std::array<std::string, SizeOfBits>>)
		length = a.size();
	//int
	else if constexpr (std::is_same_v<T, int>)
	{
		if (a == 0)
			length = 1;
		else
		{
			while (a != 0)
			{
				a /= 10;
				++length;
			}
		}
	}
	//double WITH the dot, if CountDotIfDecimal is False, then just return total-1
	else if constexpr (std::is_same_v<T, double>)
	{
		if (CountDotIfDecimal)
		{
			std::string s = std::to_string(a);
			length = s.length();
		}
		else
		{
			std::string s = std::to_string(a);
			length = s.length() - 1;
		}
	}
	//if type is not supported in here, just return compile error
	//else
	//{
	//	static_assert(false, "Type not supported in GetLengthOf. must add support for, or do something else.");
	//}
	return length;
}

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

