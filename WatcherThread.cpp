#include "WatcherThread.h"
#include "FileSystemWatcher.h"

WatcherThread::WatcherThread(FileSystemWatcher* fileSystemWatcher)
    : wxThread(wxTHREAD_JOINABLE), m_fileSystemWatcher(fileSystemWatcher)
{
}

wxThread::ExitCode WatcherThread::Entry()
{
    //wxMessageBox("WatcherThread::Entry", "", wxICON_INFORMATION);
    // Call the original StaticWatcherThread function here
    
    m_fileSystemWatcher->StaticWatcherThread(m_fileSystemWatcher);
    return (wxThread::ExitCode)0;
}

