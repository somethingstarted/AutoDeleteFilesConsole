#include "FileDeleterFrame.h"



namespace fs = std::filesystem;
DirIndexing dir_indexing;

int CreateListFromFilesLegacy(fs::path const& dir);
int DirectoryIndexer();


//whole window. replaces int main.
MyFrame::MyFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 600))
{

	


			//build the contents of whole frame
	dir_indexing.DirectoryIndexer();
	wxStaticText* staticText = DisplayCheckHDDSize();
	//wxListBox* listBox = ListDirectoryIndexer();
	wxGrid* DirAsGrid = DisplayDirectoryAsGrid();
	 

		// Set up a sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(staticText, 0, wxALIGN_LEFT | wxALL, 2);
	sizer->Add(DirAsGrid, 1, wxEXPAND | wxALL, 2);
	//sizer->Add(listBox, 1, wxEXPAND | wxALL, 2);
	SetSizer(sizer);



}



wxGrid* MyFrame::DisplayDirectoryAsGrid()
{
	wxGrid* grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	// grid UI settings
	grid->EnableDragRowMove(false);
	grid->EnableDragColSize(true);
	grid->EnableDragRowMove(false);
	grid->EnableDragColMove(true);





	// Set the number of rows and columns
	grid->CreateGrid(3, 7);
	// Set the column labels
	grid->SetColLabelValue(0, "File Name + Extension");
	grid->SetColLabelValue(1, "File Size");
	grid->SetColLabelValue(2, "File Days Old");
	grid->SetColLabelValue(3, "Date Last Modified");
	grid->SetColLabelValue(4, "read or write?");
	grid->SetColLabelValue(5, "Flagged for deletion");
	grid->SetColLabelValue(6, "Flagged for archival");

	dir_indexing.FolderIndex2.at(0).FileExtension;
	




	// Set some example data
	grid->SetCellValue(0, 0, "a1");
	grid->SetCellValue(0, 1, "A2");
	grid->SetCellValue(0, 3, "A3");
	grid->SetCellValue(0, 4, "A3");
	grid->SetCellValue(0, 5, "A3");
	grid->SetCellValue(0, 6, "A3");
	grid->SetCellValue(1, 0, "A3");
	grid->SetCellValue(1, 1, "A3");



	//formatting after the fact:
	grid->AutoSizeColumns(true);


	return grid;
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



	return staticText;
}


wxListBox* MyFrame::ListDirectoryIndexer()
{


	wxArrayString listBoxItems;

	listBoxItems.Add("Item 1");
	listBoxItems.Add("Item 2");
	listBoxItems.Add("Item 3");

	wxListBox* listBox = new wxListBox(this, wxID_ANY, wxPoint(50, 100), wxDefaultSize, listBoxItems);

	return listBox;
}