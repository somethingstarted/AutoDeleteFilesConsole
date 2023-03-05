#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <Windows.h>
#include <chrono>
#include <sstream>  
#include<tuple>

typedef   std::int64_t int64;
typedef   std::int32_t int32;
typedef   std::int16_t int16;
typedef   std::int8_t int8;

namespace fs = std::filesystem;
using chrono_ftt = std::filesystem::file_time_type;

int CreateListFromFiles();
int8 CheckHDDSizeAndSpace(fs::path, bool);
int DirectoryIndexer();
void SortListChronologically();
void CheckForDeletedFilesInVector();
int64 find_greatest(std::string, std::string, std::string);
std::string IntergerWithCommas(int64 vv);


struct FileMetaData
{
	
public:
	int OriginalFileOrder = 0;
	fs::path FileRootPath;
	fs::path FileRelativePath;
	fs::path FileName;
	fs::path FileExtension;
	bool HasPermissionToEdit;	//convert these bools to bitwise someday
	bool IsFlaggedForDeletion;
	bool FlaggedForSaving;		//never delete, move to different folder?
	unsigned long long int FileSizeKB;
	chrono_ftt FileLastModified;

};

