#pragma once
#include <bitset>
#include <filesystem>
#include <chrono>
//#include "FileDeleterFrame.h"

namespace fs = std::filesystem;
using chrono_ftt = std::filesystem::file_time_type;
using chrono_duration = std::chrono::system_clock::duration;

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

};

const int SizeOfBits = 16;
std::string IntergerWithCommas(int64);
std::string BitsWithSpaces(std::bitset<SizeOfBits>);