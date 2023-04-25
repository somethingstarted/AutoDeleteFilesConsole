#include "AutoFileDeleter.h"
#include "FileDeleterFrame.h"




bool AutoDeleteFiles::OnInit() {
	logging_tool->AppendToLog("AutoDeleteFiles::OnInit()", OutputType::SkipLine); //never delete. 

	Formatting formatting;
	DirIndexing indexer(formatting);
	auto MyPathHere = std::filesystem::current_path().string();
	std::wstring MyPathWstring = formatting.StringToWstring(MyPathHere);

	logging_tool->AppendToLog("Instantiating FileSystemWatcher", OutputType::VERBOSE); 
	FileSystemWatcher fswatcher(MyPathWstring, indexer);
	//FileSystemWatcher fswatcher;  


	logging_tool->AppendToLog("Instantiating FileDeleterFrame", OutputType::VERBOSE);
	FileDeleterFrame* frame = new FileDeleterFrame("Auto File Deleter", indexer, formatting, MyPathWstring);
	
	
	
	logging_tool->AppendToLog("Showing frame", OutputType::VERBOSE);
	frame->Show(true);
	logging_tool->AppendToLog("Starting FileSystemWatcher", OutputType::VERBOSE);
	frame->StartFileSystemWatcher();
	//frame->StartFileSystemWatcher();

	
	 

	

	return true;
}



wxIMPLEMENT_APP(AutoDeleteFiles); //only do once. 