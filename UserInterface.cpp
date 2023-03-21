#include "UserInterface.h"




bool AutoDeleteFiles::OnInit() {
	MyFrame* frame = new MyFrame("Auto File Deleter");
	frame->Show(true);
	return true;
}

//MyFrame::MyFrame(const wxString& title)
//	: wxFrame(NULL, wxID_ANY, title) {
//	// Add UI components here
//}

wxIMPLEMENT_APP(AutoDeleteFiles);



		//check hdd size
MyFrame::MyFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 600)) {
	// Create a text control with the wxTE_MULTILINE and wxTE_READONLY styles
	wxTextCtrl* textCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	// Call the CheckHDDSizeAndSpace() function and get the formatted string
	std::string output = indexer.CheckHDDSizeAndSpace(fs::current_path(), true);

	// Set the text control value
	textCtrl->SetValue(output);
}