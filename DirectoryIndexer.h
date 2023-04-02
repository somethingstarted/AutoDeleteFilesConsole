#pragma once
#include <wx/wx.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <wx/msgdlg.h>
//#include <filesystem>
//#include <Windows.h>

//#include <chrono>
#include <sstream>  
#include <tuple>
#include "Formatting.h"



//namespace fs = std::filesystem;

struct FileMetaData
{

public:
	uint64 FileIDnumber = 1;
	fs::path FileRootPath;
	fs::path FileRelativePath;
	fs::path FileName;
	fs::path FileExtension;
	int16 FileChoicesFlags;
	uint64 FileSizeKB;
	chrono_ftt TimeLastModified;

};
// DirIndexing::FolderIndex2
class DirIndexing
{
public: 
	std::vector<FileMetaData> FolderIndex2{}; //eventually store this index into a file.
	fs::path FolderToWatch = fs::current_path();	//replace current path with FolderToWatch.


	std::string CheckHDDSizeAndSpace(fs::path, bool);
	//int CreateListFromFilesLegacy(fs::path);
	int8 CheckHDDSizeAndSpaceConsole(fs::path, bool);
	uint64 DirectoryIndexBuilderUpdater();
	int64 find_greatest(std::string, std::string, std::string);
	//std::string IntergerWithCommas(int64 vv);
	int64 ListFolderIndexConsole(bool, bool, bool, bool);
	std::stringstream GetFileAge(chrono_ftt);
	int CreateListFromFilesLegacy(fs::path const& dir);
	//change to int to work with ConvertToWxArrayString
	int64 GetPercentageOf(int64, int64, bool);
	auto CalculateDailySpaceUsage();
	void SortListAlphabetically();
	std::tuple <int16, int16, int16> GetLengthOf();


	//uint64, fs::path, int16, chrono_ftt
	wxArrayString ConvertToWxArrayString(uint64);
	wxArrayString ConvertToWxArrayString(fs::path);
	wxArrayString ConvertToWxArrayString(int16);
	wxArrayString ConvertToWxArrayString(chrono_ftt);

	void SortListChronologically();
	void CheckForDeletedFilesInVector();

	//FileMetaData metadata = {};

	DirIndexing(Formatting& indexer) : formatting(indexer) {}
private:
	Formatting& formatting;


	
};





class FileFlags /* 16 bits for future impprovements*/ {
public:
	static constexpr auto ClearAll = (0x0000);

	static constexpr auto ff_CanBeWrittenTo =		(1 << 0); // if zero, ignore;
	static constexpr auto ff_IsReadyToBeDeleted = (1 << 1); // if flagged, do some checks(?) and add file info to log, then remove;
	static constexpr auto ff_ShouldBeArchived = (1 << 2); // never delete, move to different folder?;
	static constexpr auto Bit3 =			(1 << 3);	//unused for now. 

	static constexpr auto Bit4 = (1 << 4); //-----todo: let us know if file is checked as "hidden" or not. 
	static constexpr auto Bit5 = (1 << 5); //all below unused, but could be used someday if needed.
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


	//uint64, fs::path, int16, chrono_ftt
	