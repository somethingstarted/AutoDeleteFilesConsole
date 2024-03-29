#include "AutoFileDeleter.h"
#include "FileDeleterFrame.h"





bool AutoDeleteFiles::OnInit() 
{
	logging_tool->AppendToLog("AutoDeleteFiles::OnInit()", OutputType::SkipLine, WhichClassUsed::AutoDeleteFiles_Which); //never delete. 

	Formatting formatting;
	DirIndexing indexer(formatting);
	auto MyPathHere = std::filesystem::current_path().string();
	std::wstring MyPathWstring = formatting.StringToWstring(MyPathHere);

	logging_tool->AppendToLog("Instantiating FileSystemWatcher", OutputType::VERBOSE, WhichClassUsed::AutoDeleteFiles_Which);
	FileSystemWatcher fswatcher(MyPathWstring, indexer);
	//FileSystemWatcher fswatcher;  


	logging_tool->AppendToLog("Instantiating FileDeleterFrame", OutputType::VERBOSE, WhichClassUsed::AutoDeleteFiles_Which);
	FileDeleterFrame* frame = new FileDeleterFrame("Auto File Deleter", indexer, formatting, MyPathWstring);
	
	
	
	logging_tool->AppendToLog("Showing frame", OutputType::VERBOSE, WhichClassUsed::AutoDeleteFiles_Which);
	frame->Show(true);
	logging_tool->AppendToLog("Starting FileSystemWatcher", OutputType::VERBOSE, WhichClassUsed::AutoDeleteFiles_Which);
	
	frame->StartFileSystemWatcher();
	//frame->StartFileSystemWatcher();

 
	// Start the WatcherThread
	watcher_Thread = new WatcherThread(&fswatcher);
	watcher_Thread->Create();
	watcher_Thread->Run();





	

	return true;
}

//CREATE StartThreadManager();


wxIMPLEMENT_APP(AutoDeleteFiles); //only do once. 