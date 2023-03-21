#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <Windows.h>
#include <chrono>
#include <sstream>  
#include <tuple>
#include "Formatting.h"

class DirectoryIndexingClass
{
public: 
	std::string CheckHDDSizeAndSpace(fs::path, bool);
};



//int CreateListFromFiles(fs::path);
int8 CheckHDDSizeAndSpaceConsole(fs::path, bool);
int DirectoryIndexer();
void SortListChronologically();
void CheckForDeletedFilesInVector();
int64 find_greatest(std::string, std::string, std::string);
//std::string IntergerWithCommas(int64 vv);
int64 ListFolderIndex(bool, bool, bool, bool);
std::stringstream GetFileAge(chrono_ftt);
int64 GetPercentage(int64, int64, bool);
void CalculateDailySpaceUsage();

class FileFlags /* 16 bits for future impprovements*/ {
public:
	static constexpr auto ClearAll = (0x0000);

	static constexpr auto ff_CanBeWrittenTo =		(1 << 0); // if zero, ignore;
	static constexpr auto ff_IsReadyToBeDeleted = (1 << 1); // if flagged, do some checks(?) and add file info to log, then remove;
	static constexpr auto ff_ShouldBeArchived = (1 << 2); // never delete, move to different folder?;
	static constexpr auto Bit3 =			(1 << 3);	//unused for now. 

	static constexpr auto Bit4 = (1 << 4); //all below unused
	static constexpr auto Bit5 = (1 << 5);
	static constexpr auto Bit6 = (1 << 6);
	static constexpr auto Bit7 = (1 << 7);

	static constexpr auto Bit8 = (1 << 8);
	static constexpr auto Bit9 = (1 << 9);
	static constexpr auto Bit10 = (1 << 10);
	static constexpr auto Bit11 = (1 << 11);

	static constexpr auto Bit12 = (1 << 12);
	static constexpr auto Bit13 = (1 << 13);
	static constexpr auto Bit14 = (1 << 14);
	static constexpr auto Bit15 = (1 << 15);

};

struct FileMetaData
{
	
public:
	int OriginalFileOrder = 0;
	fs::path FileRootPath;
	fs::path FileRelativePath;
	fs::path FileName;
	fs::path FileExtension;
	int16 FileChoicesFlags;
	uint64 FileSizeKB;
	chrono_ftt TimeLastModified;

};

	