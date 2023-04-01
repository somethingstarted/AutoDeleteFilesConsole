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
	wxListBox*  ListDirectoryIndexer();
	wxStaticText*  DisplayCheckHDDSize();
	wxGrid* DisplayDirectoryAsGrid();
	void AlertBoxTest();
	wxGrid* DirectoryGrid{};
private:
	DirIndexing& indexer;
	Formatting& formatting;
	
};


class MainClass
{
public:
	std::string FileExtensionsToTrack;


};
