#include "FileDeleterFrame.h"
#include "DirectoryIndexer.h"

	//memory leak detection
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//end memory leak detection

namespace fs = std::filesystem;
DirIndexing dir_indexing;
int CreateListFromFiles(fs::path const& dir);
int DirectoryIndexer();

//whole window. replaces int main.
MyFrame::MyFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 600))
{
			//figure out how and where to use these. this names the program. 
	//auto wxAppConsoleBase::m_vendorName = "hi";        // vendor name		("acme")
	//m_vendorDisplayName, // vendor display name (e.g. "ACME Inc")		
	//m_appName,           // app name ("myapp")
	//m_appDisplayName,    // app display name ("My Application")
	//m_className;



			//build the contents of whole frame
	dir_indexing.DirectoryIndexer();
	wxStaticText* staticText = DisplayCheckHDDSize();
	wxListBox* listBox = ListDirectoryIndexer();



		// Set up a sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(staticText, 0, wxALIGN_LEFT | wxALL, 2);
	sizer->Add(listBox, 1, wxEXPAND | wxALL, 2);
	SetSizer(sizer);



}

wxStaticText* MyFrame::DisplayCheckHDDSize()
{
	//check hdd size
	// Call the CheckHDDSizeAndSpace() function and get the formatted string
	std::string output = indexer.CheckHDDSizeAndSpace(fs::current_path(), true);
	wxString outputWxStr = wxString::FromUTF8(output);
	// Create the wxStaticText control and set the text
	// Specify the desired width and height for the control
	int controlWidth = 400;
	int controlHeight = 10;
	// Create the wxStaticText control with custom size and wxST_NO_AUTORESIZE flag
	wxStaticText* staticText = new wxStaticText(this, wxID_ANY, outputWxStr, wxDefaultPosition, wxSize(controlWidth, controlHeight), wxST_NO_AUTORESIZE);
	staticText->SetBackgroundColour(wxColour(*wxWHITE));
	// set a custom font size and style - for later
	wxFont font = staticText->GetFont();
	font.SetPointSize(10);
	staticText->SetFont(font);
	// Calculate the best size for the control and set it
	wxSize bestSize = staticText->GetBestSize();
	staticText->Wrap(400);
	staticText->SetMinSize(bestSize);
	// Set up a sizer if needed
		//wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		//sizer->Add(staticText, 100, wxALIGN_LEFT | wxALL, 10);
		//SetSizer(sizer);

	return staticText;
}


wxListBox* MyFrame::ListDirectoryIndexer()
{
	// Create a wxListBox
	wxArrayString listBoxItems;
	listBoxItems.Add("Item 1");
	listBoxItems.Add("Item 2");
	listBoxItems.Add("Item 3");
	wxListBox* listBox = new wxListBox(this, wxID_ANY, wxPoint(50, 100), wxDefaultSize, listBoxItems);

	return listBox;
}