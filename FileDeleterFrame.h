#pragma once
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <iostream>
#include <stdio.h>
#include <wx/grid.h>
#include "VersionNumber.h"

//#include <vector>
//#include <string>
//#include <filesystem>
//#include <Windows.h>
//#include <conio.h>
 
#include "DirectoryIndexer.h"
//#include "DirectoryIndexBuilderUpdater.cpp"
#include "Formatting.h"

class MyFrame : public wxFrame {
public:
	MyFrame(const wxString& title, DirIndexing& dirIndexing);
	//MyFrame(); //build a destuctor someday? 

			//functions
	wxListBox*  ListDirectoryIndexer();
	wxStaticText*  DisplayCheckHDDSize();
	wxGrid* DisplayDirectoryAsGrid();
	void InsertMoreRows(const int64& fi_size);
	void PopulateGrid();



	wxGrid* DirectoryGrid;

private:
	void DebugTesterMessageBox(std::string, std::string, bool);

	 
	DirIndexing& indexer;
	Formatting& formatting;
	
	const int GridColums{};
	uint64 GridRows{};

			//if I need more then 256 colums i'll just... make it an int16. why the hell would i need more than 256 colums? i'm not remaking excel. 
					//this is simpler than using bitwise math to do 1 through 8. 
	const uint8 c_ID = 0;
	const uint8 c_Name = 1;
	const uint8 c_Size = 2;
	const uint8 c_Age = 3;
	const uint8 c_LstMod = 4;
	const uint8 c_RdWrtFlag = 5;
	const uint8 c_DeleteFlag = 6;
	const uint8 c_ArchiveFlag = 7;




};

 


class MainClass
{
public:
	std::string FileExtensionsToTrack;


};
