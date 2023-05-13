#include "WatcherThread.h"


WatcherThread::WatcherThread(FileSystemWatcher* fileSystemWatcher)
    : wxThread(wxTHREAD_JOINABLE), m_fileSystemWatcher(fileSystemWatcher)
{

        logging_tool->AppendToLog("WatcherThread::WatcherThread", OutputType::VERBOSE, WhichClassUsed::WatcherThread_Which);
  
    
}

wxThread::ExitCode WatcherThread::Entry()
{

    logging_tool->AppendToLog("WatcherThread::Entry", OutputType::VERBOSE, WhichClassUsed::WatcherThread_Which);


    m_fileSystemWatcher->StaticWatcherThread(m_fileSystemWatcher);
    return (wxThread::ExitCode)0;
}

