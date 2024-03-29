#include "FileDeleterFrame.h"



namespace fs = std::filesystem;
 
constexpr auto ID_BUTTON = 1; //refresh button



FileDeleterFrame::FileDeleterFrame(const wxString& title, DirIndexing& indexer, Formatting& formatting, const std::wstring& directory)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)),
      m_fileSystemWatcher(new FileSystemWatcher(directory, indexer)), formatting(formatting), fswatcher(fswatcher), indexer(indexer)
{
	
	auto MyPathHere = fs::current_path().string();

	std::stringstream WindowTitle;
	WindowTitle << "Auto File Deleter " << APP_VERSION;
	wxString newtitle = WindowTitle.str();
	wxTopLevelWindow::SetTitle(newtitle);

	DirectoryGrid = DirectoryGridConstructor(); //?

			//preprocessor stuff. might move to after window is built
	indexer.DirectoryIndexBuilderUpdater();

			//build the contents of whole frame
	wxStaticText* HddDetailsWindow = DisplayCheckHDDSize();
	wxGrid* DirAsGrid = DirectoryGridConstructor();

	// Add a button
	wxButton* GridRefreshButton = new wxButton(this, ID_BUTTON, wxT("Refresh Directory")); // Use ID_BUTTON instead of wxID_ANY
	GridRefreshButton->Bind(wxEVT_BUTTON, &FileDeleterFrame::OnGridRefreshButtonClicked, this); // Bind to OnGridRefreshButtonClicked
	

		// Set up a sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(HddDetailsWindow, 0, wxALIGN_LEFT | wxALL, 2);
	sizer->Add(GridRefreshButton, 0, wxALIGN_RIGHT | wxALL, 2);
	sizer->Add(DirAsGrid, 1, wxEXPAND | wxALL, 2);
	
	//sizer->Add(listBox, 1, wxEXPAND | wxALL, 2);
	SetSizer(sizer);
}

FileDeleterFrame::~FileDeleterFrame()
{// Nothing to do here, as std::unique_ptr takes care of deleting the FileSystemWatcher object
}

void FileDeleterFrame::StartFileSystemWatcher() 
{
	auto MyPathHere = fs::current_path().string();
	std::wstring MyPathWstring = formatting.StringToWstring(MyPathHere);


	FileSystemWatcher FileSystemWatcher(MyPathWstring, indexer);
	//FileSystemWatcher.StartMonitoring();
	//fswatcher.StartMonitoring();
	m_fileSystemWatcher->StartMonitoring();
}


		//for testing purposes. 
void FileDeleterFrame::DebugTesterMessageBox(std::string Message = {}, std::string Title = {}, bool AlwaysPopUp = 0)
{
	bool AllPopUpsOn = 0;
	if (AlwaysPopUp == 1 || AllPopUpsOn == 1)
	{
		//wxMessageBox(Message, Title, wxICON_INFORMATION);
	}
	logging_tool->AppendToLog("Message Box" + Message, OutputType::VERBOSE);
	
}



void FileDeleterFrame::InsertMoreRows(const int64& RowsToAdd)
{
	//int sizeofgrid = DirectoryGrid->GetNumberRows();

	//DirectoryGrid->InsertRows(sizeofgrid + 1, RowsToAdd, true);
	DirectoryGrid->AppendRows(RowsToAdd, true);

	DirectoryGrid->SetCellValue(1, 1, "set the cell value?");

	

	DirectoryGrid->Refresh();
	DirectoryGrid->Update();
}

void FileDeleterFrame::IterateThroughVector()
{
	logging_tool->AppendToLog("IterateThroughVector ", OutputType::VERBOSE);
	int vrc{};
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

		//logging tool output file name, extension, ID, size, and flags on one line
		logging_tool->AppendToLog("File Name: " + FileName_ForGrid.ToStdString() + " ID: " + FileID_ForGrid.ToStdString() + " Size: " + FileSize_ForGrid.ToStdString(), OutputType::VERBOSE);



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
}


wxGrid* FileDeleterFrame::DirectoryGridConstructor()
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
	
	//FileDeleterFrame::DebugTesterMessageBox(aa);

	// Set up  the column labels
	DirectoryGrid->SetColLabelValue(c_ID, "File ID");
	DirectoryGrid->SetColLabelValue(c_Name, "File Name + Extension");
	DirectoryGrid->SetColLabelValue(c_Size, "File Size");
	DirectoryGrid->SetColLabelValue(c_Age, "File Days Old");
	DirectoryGrid->SetColLabelValue(c_LstMod, "Date Last Modified");
	DirectoryGrid->SetColLabelValue(c_RdWrtFlag, "read/write permissions");
	DirectoryGrid->SetColLabelValue(c_DeleteFlag, "Flagged for deletion");
	DirectoryGrid->SetColLabelValue(c_ArchiveFlag, "Flagged for archival");



	PopulateGridFromVector(DirectoryGrid, GridColums);
	//formatting after the fact:
	DirectoryGrid->AutoSizeColumns(true);
 

//additional formatting
DirectoryGrid->AutoSizeColumns(true);
	return DirectoryGrid;
}

void FileDeleterFrame::PopulateGridFromVector(wxGrid* DirectoryGrid, const int& GridColums)
{
	//indexer.DirectoryIndexBuilderUpdater();


	uint16 vrc{};
	fs::path FileName_ForGrid;
	int64 fi_size{};
	fi_size = indexer.FolderIndex2.size();


	if (!indexer.FolderIndex2.empty())		//still not properly creating or accessing FolderIndex2... says it doesn't exsist? 
	{
		for (auto& metadata : indexer.FolderIndex2)
		{
			if (vrc >= DirectoryGrid->GetNumberRows())
				break;

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

			//print the info that the current row is holding into the logging_tool
			std::stringstream ss;
			ss << "\t" << FileID_ForGrid << "\t" << FileName_ForGrid << "\t" << FileSize_ForGrid << "\t" << FileAgeDays_ForGrid;
			logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileDeleterFrame_Which);

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
}

void FileDeleterFrame::OnGridRefreshButtonClicked(wxCommandEvent& event)
{
	//trigger indexer->CheckForDeletedFilesInVector()
	indexer.CheckForDeletedFilesInVector();
	logging_tool->AppendToLog("grid refresh button clicked", WhichClassUsed::FileDeleterFrame_Which );
}


void FileDeleterFrame::UpdateGridFromVector(wxGrid* DirectoryGrid, int GridRows)  
{
	if (indexer.CountChangesSinceGridRefresh <= 0)
	{
		 
		return;
	}
	else
	{

		// Clear the grid
		DirectoryGrid->ClearGrid();

		// Resize the grid to match the size of the vector
		int CurrentRows = DirectoryGrid->GetNumberRows();
		int NewRows = static_cast<int>(indexer.FolderIndex2.size());
		int HowManyRowsDelete = (CurrentRows > NewRows) ? (CurrentRows - NewRows) : 0;


		if (NewRows > CurrentRows) {
			DirectoryGrid->AppendRows(NewRows - CurrentRows);
		}
		if (CurrentRows >= 1 && HowManyRowsDelete) {
			DirectoryGrid->DeleteRows(0, HowManyRowsDelete, true);
		}

		CurrentRows = DirectoryGrid->GetNumberRows();

		//PopulateGridFromVector(DirectoryGrid, GridRows);
		PopulateGridFromVector(DirectoryGrid, CurrentRows);

		// Force the grid to refresh and update
		DirectoryGrid->ForceRefresh();
	}
	
}




wxStaticText* FileDeleterFrame::DisplayCheckHDDSize()
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

	HddDetailsWindow->SetScrollbar(wxHORIZONTAL, 0, 0, 0); 
		HddDetailsWindow->SetScrollbar(wxVERTICAL, 0, 0, 0);

	return HddDetailsWindow;


}

 void FileDeleterFrame::FetchDirectoryContents() 
{
	//FileMetaData directory_contents{};
	indexer.DirectoryIndexBuilderUpdater();


	return;
}

void FileDeleterFrame::ListDirectoryIndexer(wxCommandEvent& event)
{

	UpdateGridFromVector(DirectoryGrid, GridRows);

	return;
}