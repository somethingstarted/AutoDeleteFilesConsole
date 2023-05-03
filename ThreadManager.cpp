#include "ThreadManager.h"


ThreadManager::ThreadManager(WatcherThread* threadToMonitor, FileSystemWatcher* fileSystemWatcher)
    : wxThread(wxTHREAD_JOINABLE), m_WatcherThread(threadToMonitor), m_fileSystemWatcher(fileSystemWatcher)
{
}

ThreadManager::~ThreadManager()
{
}


wxThread::ExitCode ThreadManager::Entry()
{
    while (ShouldBeWatchingThreads == true)
    {
        logging_tool->AppendToLog("ThreadManager::Entry() just ran.", OutputType::VERBOSE, WhichClassUsed::ThreadManager_Which);
        if (!m_WatcherThread->IsRunning()) 
        {
            logging_tool->AppendToLog("ThreadManager::Entry(). -- Thread is not running", OutputType::VERBOSE, WhichClassUsed::ThreadManager_Which);
        }
        wxSleep(1);
    }
    return (wxThread::ExitCode)0;
}