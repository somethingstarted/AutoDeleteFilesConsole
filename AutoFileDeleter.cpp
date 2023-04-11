#include "AutoFileDeleter.h"
#include "FileDeleterFrame.h"

bool AutoDeleteFiles::OnInit() {
	// Instantiate the DirIndexing class
	//DirIndexing indexer;



	Formatting formatting;
	DirIndexing indexer(formatting);
	auto MyPathHere = std::filesystem::current_path().string();
	std::wstring MyPathWstring = formatting.StringToWstring(MyPathHere);

	// Instantiate the FileSystemWatcher with the required arguments
	FileSystemWatcher fswatcher(MyPathWstring, indexer);
	//FileSystemWatcher fswatcher;  


	MyProgramFrame* frame = new MyProgramFrame("Auto File Deleter", indexer, formatting, fswatcher);

	
	 

	frame->Show(true);


	
	 

	

	return true;
}



wxIMPLEMENT_APP(AutoDeleteFiles); //only do once. 