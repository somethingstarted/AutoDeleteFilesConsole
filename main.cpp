#include "main.h"
//
//
//
//
//namespace fs = std::filesystem;
//int CreateListFromFiles(fs::path const& dir);
//int DirectoryIndexer();
//
//
//
//
//void IsThisWindows()
//{
//
//	int ThisIsWindowsRight = 0;
//
//#if __linux__
//	ThisIsWindowsRight--;
//
//#elif _WIN32
//#if defined(_WIN64)
//	ThisIsWindowsRight++;
//	std::cout << "Win 64-bit? yes. good.\n";
//#else
//	std::cout << "Win 32-bit? yes. good.\n";
//	ThisIsWindowsRight++;
//#endif
//#else
//	ThisIsWindowsRight--;
//#endif
//
//	if (ThisIsWindowsRight <= 0)
//	{
//		std::cout << "sorry, this is not windows and may do bad things. exiting is reccomended...";
//		std::cout << "\t\npress any button to continue... \n";
//
//		std::cin.get();
//	}
//	return;
//}
//
//
//
//
//
//
//
//
//
//
////whole window. 
//MyFrame::MyFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 600))
{

	//check hdd size
// Call the CheckHDDSizeAndSpace() function and get the formatted string
	std::string output = indexer.CheckHDDSizeAndSpace(fs::current_path(), true);
	wxString outputWxStr = wxString::FromUTF8(output);
	// Create the wxStaticText control and set the text
	// Specify the desired width and height for the control
	int controlWidth = 400;
	int controlHeight = 100;
	// Create the wxStaticText control with custom size and wxST_NO_AUTORESIZE flag
	wxStaticText* staticText = new wxStaticText(this, wxID_ANY, outputWxStr, wxDefaultPosition, wxSize(controlWidth, controlHeight), wxST_NO_AUTORESIZE);
	// set a custom font size and style - for later
	wxFont font = staticText->GetFont();
	font.SetPointSize(10);
	staticText->SetFont(font);
	// Set up a sizer if needed
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(staticText, 500, wxALIGN_LEFT | wxALL, 10);
	SetSizer(sizer);

}