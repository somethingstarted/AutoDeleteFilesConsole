#include "FileDeleterFrame.h"



namespace fs = std::filesystem;
//DirIndexing dir_indexing;    
//Formatting formatting;


//int CreateListFromFilesLegacy(fs::path const& dir);
//int DirectoryIndexBuilderUpdater();

 
//whole window. replaces int main.
MyFrame::MyFrame(const wxString& title, DirIndexing& indexer)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1'200, 1'200)),
	indexer(indexer), formatting(formatting)
{
 
	std::stringstream WindowTitle;
	WindowTitle << "Auto File Deleter " << APP_VERSION;
	wxString newtitle = WindowTitle.str();
	wxTopLevelWindow::SetTitle(newtitle);



			//build the contents of whole frame
	indexer.DirectoryIndexBuilderUpdater();
	wxStaticText* staticText = DisplayCheckHDDSize();
	//wxListBox* listBox = ListDirectoryIndexer();
	wxGrid* DirAsGrid = DisplayDirectoryAsGrid();
	 

		// Set up a sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(staticText, 0, wxALIGN_LEFT | wxALL, 2);
	sizer->Add(DirAsGrid, 1, wxEXPAND | wxALL, 2);
	//sizer->Add(listBox, 1, wxEXPAND | wxALL, 2);
	SetSizer(sizer);

	
	GetBestSize();



}
		//for testing purposes. 
void MyFrame::AlertBoxTest() 
{
	wxMessageBox(wxT("Hello World!"));

	int i = 7;
	wxString Foobar;
	Foobar.Printf(wxT("Hello I have %d cookies."), i);
	wxMessageBox(Foobar);

	wxMessageBox(wxT("This is the message."), wxT("This is the title"), wxICON_INFORMATION);
}

wxGrid* MyFrame::DisplayDirectoryAsGrid()
{
	//should the directory indexer run every time this runs? or should they be seperate? 

	wxGrid* DirectoryGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	// DirectoryGrid UI settings
	DirectoryGrid->EnableDragRowMove(false);
	DirectoryGrid->EnableDragColSize(true);
	DirectoryGrid->EnableDragRowMove(false);
	DirectoryGrid->EnableDragColMove(true);
	DirectoryGrid->EnableEditing(false);



	// Set the number of rows and columns
	const int GridColums = 8;
	int GridRows = indexer.FolderIndex2.size();
	//prevent an exceptional handling
	if (GridRows <= 0)
	{
		GridRows = 1;
	}
			//apparently you have to CreateGrid before putting things in it. 
	DirectoryGrid->CreateGrid(GridRows, GridColums);


			//make const so hackers cant hack as good
	const int c_ID = 0;
	const int c_Name = 1;
	const int c_Size = 2;
	const int c_Age = 3;
	const int c_LstMod = 4;
	const int c_RdWrtFlag = 5;
	const int c_DeleteFlag = 6;
	const int c_ArchiveFlag = 7;

	// Set up  the column labels
	DirectoryGrid->SetColLabelValue(c_ID, "File ID");
	DirectoryGrid->SetColLabelValue(c_Name, "File Name + Extension");
	DirectoryGrid->SetColLabelValue(c_Size, "File Size");
	DirectoryGrid->SetColLabelValue(c_Age, "File Days Old");
	DirectoryGrid->SetColLabelValue(c_LstMod, "Date Last Modified");
	DirectoryGrid->SetColLabelValue(c_RdWrtFlag, "read/write permissions");
	DirectoryGrid->SetColLabelValue(c_DeleteFlag, "Flagged for deletion");
	DirectoryGrid->SetColLabelValue(c_ArchiveFlag, "Flagged for archival");


	
	uint16 vrc{};

	
	fs::path FileName_ForGrid;
	int64 fi_size{};
	fi_size = indexer.FolderIndex2.size();
	if (!indexer.FolderIndex2.empty())		//still not properly creating or accessing FolderIndex2... says it doesn't exsist? 
	{
		for (auto& metadata : indexer.FolderIndex2)
		{
		
			//take an int, convert to c++ string, then convert c++ string to a wxString. 

			int IDnum = indexer.FolderIndex2.at(vrc).FileIDnumber;
			wxString FileID_ForGrid = formatting.IntergerWithCommas(IDnum) + " ";

			//wxString FileID_ForGrid = std::to_string(indexer.FolderIndex2.at(vrc).FileIDnumber);
			//FileID_ForGrid = formatting.IntergerWithCommas(FileID_ForGrid);

			//take file name that's type FS::path, convert to c++ string, then convert to wxString. 
			wxString FileName_ForGrid = indexer.FolderIndex2.at(vrc).FileName.string(); // Use 'fi_size - 1' instead of 'fi_size'
			//take file size that's int, convert to c++ string, convert to wxString
			wxString FileSize_ForGrid = std::to_string(indexer.FolderIndex2.at(vrc).FileSizeKB);


			wxString FileAgeDays_ForGrid{};
			std::stringstream TheAge = formatting.GetFileAge(indexer.FolderIndex2.at(vrc).TimeLastModified);
			FileAgeDays_ForGrid = TheAge.str();


			// chrono_ftt > to stringstream > to c++ string > to wxString
			std::stringstream LastModifiedBuffer{};
			LastModifiedBuffer << std::format("{:%a %F %r }", indexer.FolderIndex2.at(vrc).TimeLastModified);
			wxString FileLastModified_ForGrid = LastModifiedBuffer.str();

			//read write 
			wxString ReadWritePrintout{};
			if (indexer.FolderIndex2.at(vrc).FileChoicesFlags & FileFlags::ff_CanBeWrittenTo)
				ReadWritePrintout = "Write";
			else
				ReadWritePrintout = "Read";

			//flagged for deletion
			bool CellRedWarning{};
			wxString FlaggedDeletionPrintout{};
			if (indexer.FolderIndex2.at(vrc).FileChoicesFlags & FileFlags::ff_IsReadyToBeDeleted)
			{
				FlaggedDeletionPrintout = "Yes";
				CellRedWarning = true;
			}
			else
			{
				FlaggedDeletionPrintout = "No";
				CellRedWarning = false;
			}
			bool CellGreenWarning{};
			wxString FlaggedArchivalPrintout{};
			if (indexer.FolderIndex2.at(vrc).FileChoicesFlags & FileFlags::ff_ShouldBeArchived)
			{
				FlaggedArchivalPrintout = "To Be Archived";
				CellGreenWarning = true;
			}
			else
			{
				FlaggedArchivalPrintout = " ";
				CellGreenWarning = false;
			}
			
			
			//put that row into a column.
			DirectoryGrid->SetCellValue(vrc, c_ID, FileID_ForGrid);
			DirectoryGrid->SetCellValue(vrc, c_Name, FileName_ForGrid);
			DirectoryGrid->SetCellValue(vrc, c_Size, FileSize_ForGrid);
			DirectoryGrid->SetCellValue(vrc, c_Age, FileAgeDays_ForGrid);
			DirectoryGrid->SetCellValue(vrc, c_LstMod, FileLastModified_ForGrid);
			DirectoryGrid->SetCellValue(vrc, c_RdWrtFlag, ReadWritePrintout);
			DirectoryGrid->SetCellValue(vrc, c_DeleteFlag, FlaggedDeletionPrintout);
			DirectoryGrid->SetCellValue(vrc, c_ArchiveFlag, FlaggedArchivalPrintout);

			//extra grid formatting:
			DirectoryGrid->SetCellAlignment(vrc, c_ID, wxALIGN_RIGHT, wxALIGN_CENTRE);
			
					//set line to red if set to be deleted.
			if (CellRedWarning == true)
			{
				for (int i = 0; i <= GridColums; i++)
				{
					DirectoryGrid->SetCellTextColour(vrc, i, "red");
				}
			}
			
					//set line green if flagged for archival.
			if (CellGreenWarning == true)
			{
				for (int i = 0; i <= GridColums; i++)
				{
					DirectoryGrid->SetCellTextColour(vrc, i, "NAVY");
					//DirectoryGrid->SetCellBackgroundColour(vrc, i, "FOREST GREEN");
				}
			}



			vrc++;
		}

		vrc = 0;
	}
	else
	{
		DirectoryGrid->SetCellValue(0, 0, "no files to put in here.");
	}



	//std::stringstream Foobar{};
	//Foobar << "FileName_ForGrid:" << FileName_ForGrid.string() << "\nfi_size: " << fi_size << "\n mem address: " << &indexer.FolderIndex2;
	//wxMessageBox(Foobar.str());


	// Set some example data
	/*DirectoryGrid->SetCellValue(0, 0, DooDoo);*/
	//DirectoryGrid->SetCellValue(0, 1, "A2");
	//DirectoryGrid->SetCellValue(0, 3, "A3");
	//DirectoryGrid->SetCellValue(0, 4, "A3");
	//DirectoryGrid->SetCellValue(0, 5, "A3");
	//DirectoryGrid->SetCellValue(0, 6, "A3");
	//DirectoryGrid->SetCellValue(1, 0, "A3");
	//DirectoryGrid->SetCellValue(1, 1, "A3");



	//formatting after the fact:
	DirectoryGrid->AutoSizeColumns(true);


	return DirectoryGrid;
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