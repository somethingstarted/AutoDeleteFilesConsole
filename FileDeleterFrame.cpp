#include "FileDeleterFrame.h"


namespace fs = std::filesystem;
 
//main window. replaces int main (doesn't work in "release build mode" though).
MyFrame::MyFrame(const wxString& title, DirIndexing& indexer)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1'200, 1'200)),
	indexer(indexer), formatting(formatting)
{
 
	std::stringstream WindowTitle;
	WindowTitle << "Auto File Deleter " << APP_VERSION;
	wxString newtitle = WindowTitle.str();
	wxTopLevelWindow::SetTitle(newtitle);

	DirectoryGrid = DisplayDirectoryAsGrid(); //?


			//preprocessor stuff. might move to after window is built
	indexer.DirectoryIndexBuilderUpdater();


			//build the contents of whole frame
	wxStaticText* HddDetailsWindow = DisplayCheckHDDSize();
	wxGrid* DirAsGrid = DisplayDirectoryAsGrid();
	 

		// Set up a sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(HddDetailsWindow, 0, wxALIGN_LEFT | wxALL, 2);
	sizer->Add(DirAsGrid, 1, wxEXPAND | wxALL, 2);
	//sizer->Add(listBox, 1, wxEXPAND | wxALL, 2);
	SetSizer(sizer);

	
	GetBestSize();



}

		//for testing purposes. 
void MyFrame::DebugTesterMessageBox(std::string Message = {}, std::string Title = {}, bool AlwaysPopUp = 0)
{
	bool AllPopUpsOn = 0;
	if (AlwaysPopUp == 1 || AllPopUpsOn == 1)
	{
		wxMessageBox(Message, Title, wxICON_INFORMATION);
	}

	
}

void MyFrame::InsertMoreRows(const int64& RowsToAdd)
{
	int sizeofgrid = DirectoryGrid->GetNumberRows();

	//DirectoryGrid->InsertRows(0, sizeofgrid + 1, true);
	 

	DirectoryGrid->SetCellValue(0, c_ID, "...");

	DirectoryGrid->Refresh();
}

void MyFrame::PopulateGrid()
{

	DebugTesterMessageBox("update index");
	indexer.DirectoryIndexBuilderUpdater();

	fs::path FileName_ForGrid;
	int64 fi_size{};

	DebugTesterMessageBox("clear grid");
	// Clear the grid before repopulating it
	DirectoryGrid->ClearGrid();
	DirectoryGrid->Refresh();
	DebugTesterMessageBox("clear grid 2");

	if (DirectoryGrid->GetNumberRows() > 0)
	{
		DirectoryGrid->DeleteRows(0, DirectoryGrid->GetNumberRows());
	}
	else if (DirectoryGrid->GetNumberRows() == 0)
	{
		GridRows = 1;
	}
	 
	// Your code to populate the grid goes here, e.g.:
	if (!indexer.FolderIndex2.empty())
	{





		uint16 vrc{}; //count amount of rows added
		

		for (auto& metadata : indexer.FolderIndex2)
		{
 

			int IDnum = indexer.FolderIndex2.at(vrc).FileIDnumber;


			std::stringstream IDnumString{};
			IDnumString << std::right << std::setw(6) << formatting.IntergerWithCommas(IDnum) << " ";
			wxString FileID_ForGrid = IDnumString.str();


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
				for (int i = 0; i <=  GridColums; i++)
				{
					DirectoryGrid->SetCellTextColour(vrc, i, "red");
				}
			}

			//set line green if flagged for archival.
			if (CellGreenWarning == true)
			{
				for (int i = 0; i <=  GridColums; i++)
				{
					DirectoryGrid->SetCellTextColour(vrc, i, "NAVY");
					//DirectoryGrid->SetCellBackgroundColour(vrc, i, "FOREST GREEN");
				}
			}



			vrc++;
		}
		 
		//DirectoryGrid->EndBatch();
	}
	else
	{
		DirectoryGrid->SetCellValue(0, 0, "no files to put in here.");
	}

	// Formatting after populating the grid:
	DirectoryGrid->AutoSizeColumns(true);

	
}

wxGrid* MyFrame::DisplayDirectoryAsGrid()
{
	// Set the number of rows and columns
	const int GridColums = 8;
	int GridRows = indexer.FolderIndex2.size();

	//prevent an exceptional handling
	if (GridRows <= 0) //this should be a template 
	{
		GridRows = 1;
	}

	wxGrid* DirectoryGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	// DirectoryGrid UI settings
	DirectoryGrid->EnableDragRowMove(false);
	DirectoryGrid->EnableDragColSize(true);
	DirectoryGrid->EnableDragRowMove(false);
	DirectoryGrid->EnableDragColMove(true);
	DirectoryGrid->EnableEditing(false);

	//apparently you have to CreateGrid before putting things in it. 
	DirectoryGrid->CreateGrid(GridRows, GridColums);

	int a = DirectoryGrid->GetNumberRows();
	std::string aa = "number of rows: " + std::to_string(a);
	MyFrame::DebugTesterMessageBox(aa);

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
	//formatting after the fact:
	DirectoryGrid->AutoSizeColumns(true);
 

//additional formatting
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
	wxStaticText* HddDetailsWindow = new wxStaticText(this, wxID_ANY, outputWxStr, wxDefaultPosition, wxSize(controlWidth, controlHeight), wxST_NO_AUTORESIZE);
	HddDetailsWindow->SetBackgroundColour(wxColour(*wxWHITE));
	// set a custom font size and style - for later
	wxFont font = HddDetailsWindow->GetFont();
	font.SetPointSize(10);
	HddDetailsWindow->SetFont(font);
	// Calculate the best size for the control and set it
	wxSize bestSize = HddDetailsWindow->GetBestSize();
	HddDetailsWindow->Wrap(400);
	HddDetailsWindow->SetMinSize(bestSize);



	return HddDetailsWindow;


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