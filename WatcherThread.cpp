#include "WatcherThread.h"
#include "FileSystemWatcher.h"

WatcherThread::WatcherThread(FileSystemWatcher* fileSystemWatcher)
    : wxThread(wxTHREAD_JOINABLE), m_fileSystemWatcher(fileSystemWatcher)
{

        logging_tool->AppendToLog("WatcherThread::WatcherThread", OutputType::VERBOSE, WhichClassUsed::WatcherThread_Which);
  
    
}

wxThread::ExitCode WatcherThread::Entry()
{
    //wxMessageBox("WatcherThread::Entry", "", wxICON_INFORMATION);
    // Call the original StaticWatcherThread function here
    logging_tool->AppendToLog("WatcherThread::Entry", OutputType::VERBOSE, WhichClassUsed::WatcherThread_Which);
    //report size of vector indexer FolderIndex2
    std::stringstream ss{};
    ss << "FolderIndex2 size: " << m_fileSystemWatcher->GetFolderIndexSize();
    logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::WatcherThread_Which);


    m_fileSystemWatcher->StaticWatcherThread(m_fileSystemWatcher);
    return (wxThread::ExitCode)0;
}

