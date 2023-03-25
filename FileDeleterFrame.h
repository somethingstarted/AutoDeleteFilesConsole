#pragma once
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <iostream>
#include <stdio.h>

//#include <vector>
//#include <string>
//#include <filesystem>
//#include <Windows.h>
//#include <conio.h>
 
#include "DirectoryIndexer.h"
#include "Formatting.h"

class MyFrame : public wxFrame {
public:
	MyFrame(const wxString& title);

	wxListBox*  ListDirectoryIndexer();
	wxStaticText*  DisplayCheckHDDSize();

private:
	DirIndexing indexer;
};


class MainClass
{
public:
	std::string FileExtensionsToTrack;


};
